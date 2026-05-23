#pragma once
#include <string>
#include <map>
#include <ctime>
#include "card.h"
#include "account.h"
#include "journal.h"

class AtmEngine {
public:
    explicit AtmEngine(Journal& journal);
    
    // Основные операции
    bool insertCard(const std::string& cardNumber);
    bool enterPin(const std::string& pin);
    double checkBalance() const;
    bool withdraw(double amount);
    bool deposit(double amount);
    void showLastTransactions() const;
    void logout();
    
    // Вспомогательные методы
    bool isAuthenticated() const { return authenticated; }
    std::string getCurrentCard() const { return currentCard; }
    
    // Администрирование (только одна декларация!)
    void addTestCard(const std::string& number, const std::string& pin, double balance);
    void resetDailyLimits();

private:
    double calculateCommission(double amount) const;
    bool checkDailyLimit(double amount) const;
    void updateDailyWithdrawn(double amount);
    std::string getCurrentDate() const;
    bool validateAmount(double amount, bool isWithdraw) const;
    
    std::map<std::string, Card> cards;
    std::map<std::string, Account> accounts;
    Journal& journal;
    std::string currentCard;
    bool authenticated = false;
    
    // Лимиты
    static constexpr double MAX_WITHDRAW_PER_TRANSACTION = 10000.0;
    static constexpr double MAX_WITHDRAW_PER_DAY = 50000.0;
    static constexpr double COMMISSION_RATE = 0.01;
    static constexpr double MIN_COMMISSION = 30.0;
    static constexpr int MAX_PIN_ATTEMPTS = 3;
};