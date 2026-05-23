#include "card.h"
#include <algorithm>

Card::Card(const std::string& num, const std::string& p)
    : number(num), pinHash(p), blocked(false), pinAttempts(0) {}

std::string Card::getNumber() const { return number; }

bool Card::checkPin(const std::string& pin) const { 
    return !blocked && pinHash == pin; 
}

bool Card::isBlocked() const { return blocked; }
void Card::block() { blocked = true; }
void Card::resetPinAttempts() { pinAttempts = 0; }
void Card::incrementPinAttempts() { pinAttempts++; }
int Card::getPinAttempts() const { return pinAttempts; }
void Card::setPinAttempts(int attempts) { pinAttempts = attempts; }