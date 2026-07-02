#include "money.hpp"

std::string currencyToString(CurrencyWrapper currency) {
    return CURRENCY_TO_STRING[currency.asIndex()];
}

unsigned int Cost::forGem(GemWrapper gem) const {
    return costs[gem.asIndex()];
}

std::string Cost::toString() const {
    return (std::ostringstream() << "C"
            << costs[0] << costs[1] << costs[2] << costs[3] << costs[4]).str();
}

unsigned int Bank::getBalance(CurrencyWrapper currency) const {
    return currencies[currency.asIndex()];
}

unsigned int Bank::totalWealth() const {
    return std::accumulate(currencies.begin(), currencies.end(), 0U);
}

unsigned int Bank::deposit(CurrencyWrapper currency, unsigned int amount) {
    return currencies[currency.asIndex()] += amount;
}

bool Bank::canAfford(const Cost& cost) const {
    for (Gem gem : GEMS) {
        if (this->getBalance(gemToCurrency(gem)) < cost.forGem(gem)) {
            return false;
        }
    }

    return true;
}

void Bank::tryWithdraw(CurrencyWrapper currency, unsigned int amount) noexcept(false) {
    if (currencies[currency.asIndex()] >= amount) {
        currencies[currency.asIndex()] -= amount;
    }
    else {
        throw InsufficientFundsException();
    }
}

void Bank::tryWithdraw(const Cost& cost) noexcept(false) {
    if (!this->canAfford(cost)) {
        throw InsufficientFundsException();
    }

    for (Gem gem : GEMS) {
        this->tryWithdraw(gemToCurrency(gem), cost.forGem(gem));
    }
}

std::string Bank::toString() const {
    return (std::ostringstream() << "B"
            << currencies[0] << currencies[1] << currencies[2] << currencies[3]
            << currencies[4] << currencies[5]).str();
}
