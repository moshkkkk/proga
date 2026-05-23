#include "atm_engine.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cmath>

AtmEngine::AtmEngine(Journal& journal) : journal(journal) {
    // Добавляем тестовые карты
    addTestCard("123456789", "1234", 50000.0);
    addTestCard("1111222233334444", "0000", 100000.0);
    addTestCard("9999999999999999", "9999", 1000.0);
}

// Реализация метода addTestCard (только один раз!)
void AtmEngine::addTestCard(const std::string& number, const std::string& pin, double balance) {
    cards[number] = Card(number, pin);
    accounts[number] = Account(number, balance);
    std::cout << "[TEST] Добавлена тестовая карта: " << number 
              << " (PIN: " << pin << ", баланс: " << balance << " RUB)" << std::endl;
}

bool AtmEngine::insertCard(const std::string& cardNumber) {
    if (cardNumber.empty()) {
        std::cout << "❌ Номер карты не может быть пустым!" << std::endl;
        return false;
    }
    
    auto it = cards.find(cardNumber);
    if (it == cards.end()) {
        std::cout << "❌ Карта с номером " << cardNumber << " не найдена!" << std::endl;
        return false;
    }
    
    if (it->second.isBlocked()) {
        std::cout << "❌ Карта заблокирована! Обратитесь в банк." << std::endl;
        return false;
    }
    
    currentCard = cardNumber;
    authenticated = false;
    std::cout << "✅ Карта принята. Введите PIN:" << std::endl;
    return true;
}

bool AtmEngine::enterPin(const std::string& pin) {
    if (currentCard.empty()) {
        std::cout << "❌ Сначала вставьте карту!" << std::endl;
        return false;
    }
    
    auto& card = cards[currentCard];
    
    if (card.isBlocked()) {
        std::cout << "❌ Карта заблокирована!" << std::endl;
        return false;
    }
    
    if (card.checkPin(pin)) {
        card.resetPinAttempts();
        authenticated = true;
        std::cout << "✅ PIN верный. Добро пожаловать!" << std::endl;
        return true;
    } else {
        card.incrementPinAttempts();
        int remaining = MAX_PIN_ATTEMPTS - card.getPinAttempts();
        
        if (card.getPinAttempts() >= MAX_PIN_ATTEMPTS) {
            card.block();
            std::cout << "❌ КАРТА ЗАБЛОКИРОВАНА! Неверный PIN 3 раза." << std::endl;
        } else {
            std::cout << "❌ Неверный PIN. Осталось попыток: " << remaining << std::endl;
        }
        return false;
    }
}

double AtmEngine::checkBalance() const {
    if (!authenticated) {
        std::cout << "❌ Необходимо авторизоваться!" << std::endl;
        return -1;
    }
    
    double balance = accounts.at(currentCard).getBalance();
    std::cout << "💰 Ваш баланс: " << std::fixed << std::setprecision(2) << balance << " RUB" << std::endl;
    return balance;
}

double AtmEngine::calculateCommission(double amount) const {
    if (amount <= 0) return 0;
    double commission = amount * COMMISSION_RATE;
    if (commission < MIN_COMMISSION) commission = MIN_COMMISSION;
    return commission;
}

std::string AtmEngine::getCurrentDate() const {
    std::time_t t = std::time(nullptr);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&t), "%Y-%m-%d");
    return ss.str();
}

bool AtmEngine::checkDailyLimit(double amount) const {
    if (!authenticated) return false;
    
    std::string today = getCurrentDate();
    double withdrawnToday = accounts.at(currentCard).getDailyWithdrawn(today);
    
    return (withdrawnToday + amount) <= MAX_WITHDRAW_PER_DAY;
}

void AtmEngine::updateDailyWithdrawn(double amount) {
    if (!authenticated) return;
    
    std::string today = getCurrentDate();
    accounts[currentCard].addDailyWithdrawn(today, amount);
}

bool AtmEngine::validateAmount(double amount, bool isWithdraw) const {
    if (amount <= 0) {
        std::cout << "❌ Сумма должна быть больше 0!" << std::endl;
        return false;
    }
    
    // Проверка на слишком большие числа
    if (amount > 1e9) {
        std::cout << "❌ Слишком большая сумма!" << std::endl;
        return false;
    }
    
    // Проверка на количество знаков после запятой
    if (std::fabs(amount - std::round(amount * 100) / 100) > 0.001) {
        std::cout << "❌ Сумма должна быть с точностью до 2 знаков после запятой!" << std::endl;
        return false;
    }
    
    if (isWithdraw) {
        if (amount > MAX_WITHDRAW_PER_TRANSACTION) {
            std::cout << "❌ Максимальная сумма снятия за раз: " 
                      << MAX_WITHDRAW_PER_TRANSACTION << " RUB!" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool AtmEngine::withdraw(double amount) {
    if (!authenticated) {
        std::cout << "❌ Необходимо авторизоваться!" << std::endl;
        return false;
    }
    
    if (!validateAmount(amount, true)) return false;
    
    if (!checkDailyLimit(amount)) {
        std::cout << "❌ Превышен суточный лимит! Максимум: " 
                  << MAX_WITHDRAW_PER_DAY << " RUB в день." << std::endl;
        return false;
    }
    
    double commission = calculateCommission(amount);
    double total = amount + commission;
    
    auto& acc = accounts[currentCard];
    
    if (!acc.canWithdraw(amount, commission)) {
        std::cout << "❌ Недостаточно средств!" << std::endl;
        std::cout << "💰 Доступно: " << std::fixed << std::setprecision(2) 
                  << acc.getBalance() << " RUB" << std::endl;
        std::cout << "💰 Требуется (с комиссией): " << total << " RUB" << std::endl;
        return false;
    }
    
    // Выполняем операцию
    acc.setBalance(acc.getBalance() - total);
    Transaction t(TransType::WITHDRAW, amount, commission);
    acc.addTransaction(t);
    
    if (!journal.appendTransaction(currentCard, t)) {
        // Откат при ошибке журнала
        acc.setBalance(acc.getBalance() + total);
        std::cout << "❌ Ошибка записи в журнал! Операция отменена." << std::endl;
        return false;
    }
    
    updateDailyWithdrawn(amount);
    
    std::cout << "✅ Снятие выполнено успешно!" << std::endl;
    std::cout << "💰 Снято: " << std::fixed << std::setprecision(2) << amount << " RUB" << std::endl;
    std::cout << "💰 Комиссия: " << commission << " RUB" << std::endl;
    std::cout << "💰 Остаток: " << acc.getBalance() << " RUB" << std::endl;
    
    return true;
}

bool AtmEngine::deposit(double amount) {
    if (!authenticated) {
        std::cout << "❌ Необходимо авторизоваться!" << std::endl;
        return false;
    }
    
    if (!validateAmount(amount, false)) return false;
    
    auto& acc = accounts[currentCard];
    acc.setBalance(acc.getBalance() + amount);
    Transaction t(TransType::DEPOSIT, amount, 0);
    acc.addTransaction(t);
    
    if (!journal.appendTransaction(currentCard, t)) {
        // Откат при ошибке
        acc.setBalance(acc.getBalance() - amount);
        std::cout << "❌ Ошибка записи в журнал! Операция отменена." << std::endl;
        return false;
    }
    
    std::cout << "✅ Пополнение выполнено успешно!" << std::endl;
    std::cout << "💰 Внесено: " << std::fixed << std::setprecision(2) << amount << " RUB" << std::endl;
    std::cout << "💰 Новый баланс: " << acc.getBalance() << " RUB" << std::endl;
    
    return true;
}

void AtmEngine::showLastTransactions() const {
    if (!authenticated) {
        std::cout << "❌ Необходимо авторизоваться!" << std::endl;
        return;
    }
    
    auto trans = accounts.at(currentCard).getLastTransactions(5);
    
    if (trans.empty()) {
        std::cout << "📭 История операций пуста." << std::endl;
        return;
    }
    
    std::cout << "\n┌─────────────────────────────────────────────────┐" << std::endl;
    std::cout << "│           ПОСЛЕДНИЕ 5 ОПЕРАЦИЙ                 │" << std::endl;
    std::cout << "├─────────────────────────────────────────────────┤" << std::endl;
    
    for (const auto& t : trans) {
        std::cout << "│ " << t.toString() << std::endl;
    }
    
    std::cout << "└─────────────────────────────────────────────────┘" << std::endl;
}

void AtmEngine::logout() {
    if (authenticated) {
        std::cout << "👋 До свидания! Карта возвращена." << std::endl;
    }
    authenticated = false;
    currentCard = "";
}

void AtmEngine::resetDailyLimits() {
    // Сброс суточных лимитов (для тестирования)
    for (auto& [card, acc] : accounts) {
        // Создаём новый аккаунт с пустыми лимитами
        acc = Account(card, acc.getBalance());
    }
}