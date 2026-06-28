#include <vector>
#include <functional>
#include <assert.h>

#include "money.hpp"
#include "cards.hpp"

#ifndef __PLAYER_INCLUDE
#define __PLAYER_INCLUDE

class Player {
    std::vector<std::reference_wrapper<Card>> builtCards;
    std::vector<std::reference_wrapper<Card>> reservedCards;
    std::vector<std::reference_wrapper<Noble>> nobles;
    Bank bank;
    Bank bonuses;
    unsigned int points;

    bool canAfford(const Cost& cost);
    bool tryPay(const Cost& cost);
    void buildCard(std::reference_wrapper<Card> card);
    void visitFromNoble(std::reference_wrapper<Noble> noble);
};

#endif  // __PLAYER_INCLUDE
