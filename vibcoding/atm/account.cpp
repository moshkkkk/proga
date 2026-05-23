#include "account.h"
#include <algorithm>

Account::Account() : balance(0.0) {}

Account::Account(const std::string& cardNum, double initialBalance)
    : cardNumber(cardNum), balance(initialBalance) {}

std::string Account::getCardNumber() const { return cardNumber; }
double Account::getBalance() const { return balance; }
void Account::setBalance(double newBalance) { balance = newBalance; }

bool Account::canWithdraw(double amount, double commission) const {
    return balance >= (amount + commission);
}

void Account::addTransaction(const Transaction& t) {
    transactions.push_back(t);
    // Храним последние 100 транзакций
    while (transactions.size() > 100) {
        transactions.erase(transactions.begin());
    }
}

std::vector<Transaction> Account::getLastTransactions(int count) const {
    if (count <= 0) return {};
    int start = static_cast<int>(transactions.size()) - count;
    if (start < 0) start = 0;
    return std::vector<Transaction>(transactions.begin() + start, transactions.end());
}

void Account::clearTransactions() { 
    transactions.clear(); 
}

double Account::getDailyWithdrawn(const std::string& date) const {
    auto it = dailyWithdrawn.find(date);
    return (it != dailyWithdrawn.end()) ? it->second : 0.0;
}

void Account::addDailyWithdrawn(const std::string& date, double amount) {
    dailyWithdrawn[date] += amount;
}