#pragma once
#include <string>
#include <vector>
#include <utility>
#include "transaction.h"

class Journal {
public:
    explicit Journal(const std::string& filename);
    
    bool appendTransaction(const std::string& cardNumber, const Transaction& t);
    std::vector<std::pair<std::string, Transaction>> readAll() const;
    void clear();
    bool isEmpty() const;

private:
    std::string filename;
    
    struct JournalEntry {
        std::string cardNumber;
        Transaction transaction;
    };
};