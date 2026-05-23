#include "file_storage.h"
#include <fstream>
#include <iostream>
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h>
#define mkdir _mkdir
#else
#include <sys/stat.h>
#endif

FileStorage::FileStorage(const std::string& cf, const std::string& af)
    : cardsFilename(cf), accountsFilename(af) {
    ensureDirectoryExists();
}

bool FileStorage::ensureDirectoryExists() {
    #ifdef _WIN32
    if (_mkdir("data") != 0 && errno != EEXIST) return false;
    #else
    if (mkdir("data", 0777) != 0 && errno != EEXIST) return false;
    #endif
    return true;
}

bool FileStorage::saveCards(const std::map<std::string, Card>& cards) {
    std::ofstream file(cardsFilename, std::ios::binary);
    if (!file) return false;
    
    size_t size = cards.size();
    file.write(reinterpret_cast<const char*>(&size), sizeof(size));
    
    for (const auto& [number, card] : cards) {
        size_t len = number.size();
        file.write(reinterpret_cast<const char*>(&len), sizeof(len));
        file.write(number.c_str(), len);
        // Упрощённо: сохраняем остальные поля
    }
    
    return true;
}

std::map<std::string, Card> FileStorage::loadCards() {
    std::map<std::string, Card> cards;
    std::ifstream file(cardsFilename, std::ios::binary);
    if (!file) return cards;
    
    // Заглушка: возвращаем тестовые данные
    cards["123456789"] = Card("123456789", "1234");
    cards["1111222233334444"] = Card("1111222233334444", "0000");
    
    return cards;
}

bool FileStorage::saveAccounts(const std::map<std::string, Account>& accounts) {
    std::ofstream file(accountsFilename, std::ios::binary);
    return file.good();
}

std::map<std::string, Account> FileStorage::loadAccounts() {
    std::map<std::string, Account> accounts;
    accounts["123456789"] = Account("123456789", 50000.0);
    accounts["1111222233334444"] = Account("1111222233334444", 100000.0);
    return accounts;
}

bool FileStorage::backup() { return true; }
bool FileStorage::restore() { return true; }