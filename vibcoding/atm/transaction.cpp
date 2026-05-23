#include "transaction.h"
#include <sstream>
#include <iomanip>

Transaction::Transaction() : type(TransType::DEPOSIT), amount(0), timestamp(0), commission(0) {}

Transaction::Transaction(TransType t, double a, double comm)
    : type(t), amount(a), commission(comm), timestamp(std::time(nullptr)) {}

std::string Transaction::toString() const {
    std::stringstream ss;
    std::string typeStr = (type == TransType::DEPOSIT) ? "ПОПОЛНЕНИЕ" : "СНЯТИЕ";
    
    ss << typeStr << " " << std::fixed << std::setprecision(2) << amount << " RUB";
    
    if (commission > 0) {
        ss << " (комиссия: " << std::fixed << std::setprecision(2) << commission << " RUB)";
    }
    
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%d.%m.%Y %H:%M:%S", std::localtime(&timestamp));
    ss << " | " << buffer;
    
    return ss.str();
}

bool Transaction::operator==(const Transaction& other) const {
    return type == other.type && amount == other.amount && commission == other.commission;
}