#include "player.hpp"

bool Player::canAfford(const Cost& cost) const {
    unsigned int wildsLeft = bank.getBalance(Currency::GOLD);
    int remainder = 0;

    for (Gem gem : GEMS) {
        remainder = static_cast<int>(cost.forGem(gem))
                    - bonuses.getBalance(gem) - bank.getBalance(gem);
        if (remainder > 0) {
            if (wildsLeft >= remainder) {
                wildsLeft -= remainder;
            }
            else {
                return false;
            }
        }
    }

    return true;
}

void Player::tryPay(const Cost& cost) noexcept(false) {
    if (!canAfford(cost)) {
        throw InsufficientFundsException();
    }

    int realCost = 0;

    for (Gem gem : GEMS) {
        realCost = static_cast<int>(cost.forGem(gem)) - bonuses.getBalance(gem);

        // realCost negative -> bonuses took care of purchase
        if (realCost >= 0) {
            if (bank.getBalance(gem) >= realCost) {
                bank.tryWithdraw(gem, realCost);
            }
            else {
                if (realCost > bank.getBalance(gem)) {
                    bank.tryWithdraw(Currency::GOLD, realCost - bank.getBalance(gem));
                }
                bank.tryWithdraw(gem, bank.getBalance(gem));
            }
        }
    }
}

bool Player::canBuildCard(const Card& card) const {
    return canAfford(card.cost);
}

void Player::tryBuildCard(const Card& card) noexcept(false) {
    if (!canBuildCard(card)) {
        throw InsufficientFundsException();
    }

    tryPay(card.cost);
    builtCards.push_back(card);
    bonuses.deposit(card.output, 1);
    points += card.points;
}

bool Player::canHostNoble(const Noble& noble) const {
    return bonuses.canAfford(noble.cost);
}

void Player::hostNoble(const Noble& noble) {
    nobles.push_back(noble);
    points += noble.points;
}

bool Player::canReserve() const {
    return reservedCards.size() < MAX_RESERVATIONS;
}

void Player::tryReserve(const Card& card) {
    if (!canReserve()) {
        throw std::length_error("Player already has max reservations");
    }

    reservedCards.push_back(card);
}

bool Player::canBuildReserved(std::size_t reserveIdx) const {
    return reserveIdx < reservedCards.size()
        && canAfford(reservedCards[reserveIdx].get().cost);
}

void Player::tryBuildReserved(std::size_t reserveIdx) noexcept(false) {
    if (reserveIdx >= reservedCards.size()) {
        throw NoCardsException();
    }
    tryPay(reservedCards[reserveIdx].get().cost);
    builtCards.push_back(reservedCards[reserveIdx].get());
    reservedCards.erase(reservedCards.begin() + reserveIdx);
}

bool Player::isTooRich() const {
    return bank.totalWealth() > MAX_BANK_WEALTH;
}

std::string Player::toString() const {
    std::ostringstream stream;

    stream << "Player: \n\tCards:\n";
    for (const Card& card : builtCards) {
        stream << "\t\t" << card.toString() << "\n";
    }

    stream << "\tReserve:\n";
    for (const Card& card : reservedCards) {
        stream << "\t\t" << card.toString() << "\n";
    }

    stream << "\tNobles:\n";
    for (const Noble& noble : nobles) {
        stream << "\t\t" << noble.toString() << "\n";
    }

    stream << "\tBank: " << bank.toString() << "\n";
    stream << "\tBonuses: " << bonuses.toString() << "\n";
    stream << "\tPoints: " << points;

    return stream.str();
}
