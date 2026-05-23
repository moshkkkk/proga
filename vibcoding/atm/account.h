#pragma once
#include <string>
#include <vector>
#include <map>  // ← ДОБАВИТЬ ЭТУ СТРОКУ
#include "transaction.h"

class Account {
public:
    Account();
    explicit Account(const std::string& cardNumber, double initialBalance = 0.0);
    
    std::string getCardNumber() const;
    double getBalance() const;
    void setBalance(double newBalance);
    bool canWithdraw(double amount, double commission) const;
    void addTransaction(const Transaction& t);
    std::vector<Transaction> getLastTransactions(int count) const;
    void clearTransactions();
    double getDailyWithdrawn(const std::string& date) const;
    void addDailyWithdrawn(const std::string& date, double amount);

private:
    std::string cardNumber;
    double balance = 0.0;
    std::vector<Transaction> transactions;
    std::map<std::string, double> dailyWithdrawn; // дата -> сумма снятий
};