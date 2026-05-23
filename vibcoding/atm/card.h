#pragma once
#include <string>

class Card {
public:
    Card() = default;
    Card(const std::string& number, const std::string& pin);
    
    std::string getNumber() const;
    bool checkPin(const std::string& pin) const;
    bool isBlocked() const;
    void block();
    void resetPinAttempts();
    void incrementPinAttempts();
    int getPinAttempts() const;
    void setPinAttempts(int attempts);

private:
    std::string number;
    std::string pinHash;
    bool blocked = false;
    int pinAttempts = 0;
};