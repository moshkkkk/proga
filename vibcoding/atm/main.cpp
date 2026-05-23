#include "atm_engine.h"
#include "journal.h"
#include "recovery.h"
#include <iostream>
#include <limits>
#include <string>

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void showMainMenu() {
    std::cout << "\n╔══════════════════════════════════════╗" << std::endl;
    std::cout << "║           БАНКОМАТ v2.0              ║" << std::endl;
    std::cout << "╠══════════════════════════════════════╣" << std::endl;
    std::cout << "║ 1. Проверить баланс                 ║" << std::endl;
    std::cout << "║ 2. Снять наличные                   ║" << std::endl;
    std::cout << "║ 3. Пополнить счет                   ║" << std::endl;
    std::cout << "║ 4. История операций (последние 5)   ║" << std::endl;
    std::cout << "║ 5. Выход                            ║" << std::endl;
    std::cout << "╚══════════════════════════════════════╝" << std::endl;
    std::cout << "Выберите действие: ";
}

double readAmount(const std::string& prompt) {
    double amount;
    while (true) {
        std::cout << prompt;
        std::cin >> amount;
        
        if (std::cin.fail()) {
            std::cout << "❌ Ошибка: введите число!" << std::endl;
            clearInput();
            continue;
        }
        
        if (amount < 0) {
            std::cout << "❌ Сумма не может быть отрицательной!" << std::endl;
            continue;
        }
        
        break;
    }
    return amount;
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    
    std::cout << "\n🏦 Добро пожаловать в банкомат!" << std::endl;
    
    // Инициализация
    Journal journal("data/journal.bin");
    AtmEngine atm(journal);
    RecoveryManager recovery(journal);
    
    // Восстановление после сбоя
    std::map<std::string, Account> dummyAccounts;
    recovery.recoverAccounts(dummyAccounts);
    
    // Ввод карты
    std::string cardNum;
    std::cout << "\nВведите номер карты: ";
    std::cin >> cardNum;
    
    if (!atm.insertCard(cardNum)) {
        std::cout << "Программа завершена." << std::endl;
        return 1;
    }
    
    // Ввод PIN
    std::string pin;
    bool authorized = false;
    
    for (int attempt = 1; attempt <= 3; attempt++) {
        std::cout << "Введите PIN: ";
        std::cin >> pin;
        
        if (atm.enterPin(pin)) {
            authorized = true;
            break;
        }
        
        if (attempt < 3) {
            std::cout << "Осталось попыток: " << (3 - attempt) << std::endl;
        }
    }
    
    if (!authorized) {
        std::cout << "\n❌ Доступ запрещён. Карта заблокирована." << std::endl;
        return 1;
    }
    
    // Основной цикл
    int choice;
    do {
        showMainMenu();
        std::cin >> choice;
        
        if (std::cin.fail()) {
            clearInput();
            std::cout << "❌ Неверный ввод!" << std::endl;
            continue;
        }
        
        switch (choice) {
            case 1:
                atm.checkBalance();
                break;
                
            case 2: {
                double amount = readAmount("Введите сумму для снятия: ");
                if (amount > 0) {
                    atm.withdraw(amount);
                }
                break;
            }
            
            case 3: {
                double amount = readAmount("Введите сумму для пополнения: ");
                if (amount > 0) {
                    atm.deposit(amount);
                }
                break;
            }
            
            case 4:
                atm.showLastTransactions();
                break;
                
            case 5:
                atm.logout();
                std::cout << "Спасибо за использование банкомата!" << std::endl;
                break;
                
            default:
                std::cout << "❌ Неверный выбор! Выберите 1-5." << std::endl;
        }
    } while (choice != 5);
    
    return 0;
}