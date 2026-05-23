#include "recovery.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>

RecoveryManager::RecoveryManager(Journal& journal) 
    : journal(journal), backupFile("data/backup_" + std::to_string(std::time(nullptr)) + ".bin") {}

bool RecoveryManager::recoverAccounts(std::map<std::string, Account>& accounts) {
    std::cout << "[RECOVERY] Начинаю восстановление..." << std::endl;
    
    // Создаём бэкап текущего состояния
    createBackup();
    
    // Сохраняем старые балансы для отладки
    std::cout << "[RECOVERY] Текущие балансы:" << std::endl;
    for (const auto& [card, acc] : accounts) {
        std::cout << "   " << card << ": " << acc.getBalance() << " RUB" << std::endl;
    }
    
    // Сбрасываем все балансы
    for (auto& [card, acc] : accounts) {
        acc.setBalance(0.0);
        acc.clearTransactions();
    }
    
    // Воспроизводим транзакции
    bool success = replayTransactions(accounts);
    
    if (success) {
        std::cout << "[RECOVERY] Восстановление успешно завершено" << std::endl;
        std::cout << "[RECOVERY] Новые балансы:" << std::endl;
        for (const auto& [card, acc] : accounts) {
            std::cout << "   " << card << ": " << acc.getBalance() << " RUB" << std::endl;
        }
    } else {
        std::cout << "[RECOVERY] Ошибка при восстановлении" << std::endl;
        restoreFromBackup();
    }
    
    return success;
}

bool RecoveryManager::replayTransactions(std::map<std::string, Account>& accounts) {
    auto entries = journal.readAll();
    
    std::cout << "[RECOVERY] Найдено транзакций в журнале: " << entries.size() << std::endl;
    
    for (const auto& [cardNumber, trans] : entries) {
        if (accounts.find(cardNumber) == accounts.end()) {
            // Создаём аккаунт, если его нет
            std::cout << "[RECOVERY] Создан новый аккаунт для карты: " << cardNumber << std::endl;
            accounts[cardNumber] = Account(cardNumber, 0.0);
        }
        
        auto& acc = accounts[cardNumber];
        
        try {
            if (trans.type == TransType::DEPOSIT) {
                double oldBalance = acc.getBalance();
                acc.setBalance(oldBalance + trans.amount);
                std::cout << "[RECOVERY] " << cardNumber << ": ПОПОЛНЕНИЕ " 
                          << trans.amount << " RUB (было: " << oldBalance 
                          << " -> стало: " << acc.getBalance() << ")" << std::endl;
            } else if (trans.type == TransType::WITHDRAW) {
                double total = trans.amount + trans.commission;
                double oldBalance = acc.getBalance();
                if (oldBalance >= total) {
                    acc.setBalance(oldBalance - total);
                    std::cout << "[RECOVERY] " << cardNumber << ": СНЯТИЕ " 
                              << trans.amount << " RUB + комиссия " << trans.commission 
                              << " RUB (было: " << oldBalance 
                              << " -> стало: " << acc.getBalance() << ")" << std::endl;
                } else {
                    std::cout << "[RECOVERY] ПРЕДУПРЕЖДЕНИЕ: Недостаточно средств для " 
                              << cardNumber << " (нужно: " << total 
                              << ", есть: " << oldBalance << ")" << std::endl;
                    // Пропускаем эту транзакцию
                    continue;
                }
            }
            acc.addTransaction(trans);
        } catch (const std::exception& e) {
            std::cout << "[RECOVERY] Ошибка при воспроизведении транзакции: " << e.what() << std::endl;
            return false;
        }
    }
    
    return true;
}

void RecoveryManager::createBackup() const {
    std::ofstream backup(backupFile, std::ios::binary);
    if (backup) {
        backup.close();
        std::cout << "[RECOVERY] Бэкап создан: " << backupFile << std::endl;
    } else {
        std::cout << "[RECOVERY] НЕ УДАЛОСЬ создать бэкап: " << backupFile << std::endl;
    }
}

bool RecoveryManager::restoreFromBackup() {
    std::cout << "[RECOVERY] Восстанавливаю из бэкапа..." << std::endl;
    // Здесь можно реализовать восстановление из бэкапа
    return true;
}

bool RecoveryManager::validateAndRepair() {
    auto entries = journal.readAll();
    
    bool valid = true;
    for (const auto& [card, trans] : entries) {
        // Проверяем корректность сумм
        if (trans.amount <= 0) {
            std::cout << "[RECOVERY] Найдена некорректная транзакция: сумма " 
                      << trans.amount << " RUB" << std::endl;
            valid = false;
        }
        if (trans.type == TransType::WITHDRAW && trans.commission < 0) {
            std::cout << "[RECOVERY] Найдена некорректная комиссия: " 
                      << trans.commission << " RUB" << std::endl;
            valid = false;
        }
    }
    
    return valid;
}