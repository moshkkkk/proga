#pragma once
#include <string>
#include <map>
#include "card.h"
#include "account.h"

class FileStorage {
public:
    FileStorage(const std::string& cardsFile, const std::string& accountsFile);
    
    bool saveCards(const std::map<std::string, Card>& cards);
    std::map<std::string, Card> loadCards();
    
    bool saveAccounts(const std::map<std::string, Account>& accounts);
    std::map<std::string, Account> loadAccounts();
    
    bool backup();
    bool restore();

private:
    std::string cardsFilename;
    std::string accountsFilename;
    
    bool ensureDirectoryExists();
};