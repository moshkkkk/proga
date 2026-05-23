#pragma once
#include "journal.h"
#include "account.h"
#include <map>
#include <string>

class RecoveryManager {
public:
    explicit RecoveryManager(Journal& journal);
    
    bool recoverAccounts(std::map<std::string, Account>& accounts);
    bool validateAndRepair();
    void createBackup() const;
    bool restoreFromBackup();
    
private:
    Journal& journal;
    std::string backupFile;
    
    bool replayTransactions(std::map<std::string, Account>& accounts);
};