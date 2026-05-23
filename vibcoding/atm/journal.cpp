#include "journal.h"
#include <fstream>
#include <iostream>
#include <cstring>

Journal::Journal(const std::string& fname) : filename(fname) {}

bool Journal::appendTransaction(const std::string& cardNumber, const Transaction& t) {
    std::ofstream file(filename, std::ios::binary | std::ios::app);
    if (!file.is_open()) {
        // Пробуем создать директорию
        std::ofstream create(filename, std::ios::binary);
        if (!create) return false;
        create.close();
        file.open(filename, std::ios::binary | std::ios::app);
        if (!file) return false;
    }
    
    try {
        size_t len = cardNumber.size();
        file.write(reinterpret_cast<const char*>(&len), sizeof(len));
        file.write(cardNumber.c_str(), len);
        file.write(reinterpret_cast<const char*>(&t.type), sizeof(t.type));
        file.write(reinterpret_cast<const char*>(&t.amount), sizeof(t.amount));
        file.write(reinterpret_cast<const char*>(&t.timestamp), sizeof(t.timestamp));
        file.write(reinterpret_cast<const char*>(&t.commission), sizeof(t.commission));
        
        file.flush();
        return true;
    } catch (...) {
        return false;
    }
}

std::vector<std::pair<std::string, Transaction>> Journal::readAll() const {
    std::vector<std::pair<std::string, Transaction>> result;
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) return result;
    
    while (file.peek() != EOF) {
        size_t len = 0;
        if (!file.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
        
        if (len == 0 || len > 100) break; // Защита от битых данных
        
        std::string cardNumber(len, '\0');
        if (!file.read(&cardNumber[0], len)) break;
        
        Transaction t;
        if (!file.read(reinterpret_cast<char*>(&t.type), sizeof(t.type))) break;
        if (!file.read(reinterpret_cast<char*>(&t.amount), sizeof(t.amount))) break;
        if (!file.read(reinterpret_cast<char*>(&t.timestamp), sizeof(t.timestamp))) break;
        if (!file.read(reinterpret_cast<char*>(&t.commission), sizeof(t.commission))) break;
        
        result.emplace_back(cardNumber, t);
    }
    
    return result;
}

void Journal::clear() {
    std::ofstream file(filename, std::ios::binary | std::ios::trunc);
    if (file) file.close();
}

bool Journal::isEmpty() const {
    std::ifstream file(filename, std::ios::binary);
    file.seekg(0, std::ios::end);
    return file.tellg() <= 0;
}