#include "money.hpp"

unsigned int Cost::forGem(GemWrapper gem) const {
    return costs[gem.asIndex()];
}

unsigned int Bank::getBalance(CurrencyWrapper currency) {
    return currencies[currency.asIndex()];
}

unsigned int Bank::deposit(CurrencyWrapper currency, unsigned int amount) {
    return currencies[currency.asIndex()] += amount;
}

bool Bank::tryWithdraw(CurrencyWrapper currency, unsigned int amount) {
    if (currencies[currency.asIndex()] >= amount) {
        currencies[currency.asIndex()] -= amount;
        return true;
    }
    return false;
}

bool Bank::canAfford(const Cost& cost) {
    for (Gem gem : GEMS) {
        if (this->getBalance(gemToCurrency(gem)) < cost.forGem(gem)) {
            return false;
        }
    }

    return true;
}

bool Bank::tryWithdraw(const Cost& cost) {
    if (!this->canAfford(cost)) {
        return false;
    }

    for (Gem gem : GEMS) {
        assert(this->tryWithdraw(gemToCurrency(gem), cost.forGem(gem)));
    }

    return true;
}

std::string Cost::toString() const {
    return (std::ostringstream() <<
            costs[0] << costs[1] << costs[2] << costs[3] << costs[4]).str();
}
