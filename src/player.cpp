#include "player.hpp"

bool Player::canAfford(const Cost& cost) {
    unsigned int wildsLeft = bank.getBalance(Currency::GOLD);
    int remainder = 0;

    for (Gem gem : GEMS) {
        remainder = static_cast<int>(cost.forGem(gem)) - bonuses.getBalance(gem) - bank.getBalance(gem);
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

bool Player::tryPay(const Cost& cost) {
    if (!canAfford(cost)) {
        return false;
    }

    int realCost = 0;

    for (Gem gem : GEMS) {
        realCost = static_cast<int>(cost.forGem(gem)) - bonuses.getBalance(gem);

        // realCost negative -> bonuses took care of purchase
        if (realCost >= 0 && !bank.tryWithdraw(gem, realCost)) {
            if (realCost > bank.getBalance(gem)) {
                assert(bank.tryWithdraw(Currency::GOLD, realCost - bank.getBalance(gem)));
            }
            assert(bank.tryWithdraw(gem, bank.getBalance(gem)));
        }
    }

    return true;
}

void Player::buildCard(std::reference_wrapper<Card> card) {
    builtCards.push_back(card);
    bonuses.deposit(card.get().output, 1);
    points += card.get().points;
}

void Player::visitFromNoble(std::reference_wrapper<Noble> noble) {
    nobles.push_back(noble);
    points += noble.get().points;
}
