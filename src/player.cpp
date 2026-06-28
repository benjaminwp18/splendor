#include "player.hpp"

bool Player::canAfford(const Cost& cost) {
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

void Player::buildCard(Card& card) {
    builtCards.push_back(card);
    bonuses.deposit(card.output, 1);
    points += card.points;
}

bool Player::canHostNoble(Noble& noble) {
    return bonuses.canAfford(noble.cost);
}

void Player::hostNoble(Noble& noble) {
    nobles.push_back(noble);
    points += noble.points;
}

std::string Player::toString() const {
    std::ostringstream stream;

    stream << "Player: \n\tCards:\n";
    for (CardRef card : builtCards) {
        stream << "\t\t" << card.get().toString() << "\n";
    }

    stream << "\tReserve:\n";
    for (CardRef card : reservedCards) {
        stream << "\t\t" << card.get().toString() << "\n";
    }

    stream << "\tNobles:\n";
    for (NobleRef noble : nobles) {
        stream << "\t\t" << noble.get().toString() << "\n";
    }

    stream << "\tBank: " << bank.toString() << "\n";
    stream << "\tBonuses: " << bonuses.toString() << "\n";
    stream << "\tPoints: " << points;

    return stream.str();
}
