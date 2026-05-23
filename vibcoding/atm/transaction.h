#pragma once
#include <string>
#include <ctime>

enum class TransType { DEPOSIT, WITHDRAW };

struct Transaction {
    TransType type;
    double amount;
    std::time_t timestamp;
    double commission = 0.0;
    
    Transaction();
    Transaction(TransType t, double a, double comm = 0.0);
    std::string toString() const;
    
    bool operator==(const Transaction& other) const;
};