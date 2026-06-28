#include <vector>
#include <functional>
#include <assert.h>
#include <numeric>
#include <sstream>

#include "money.hpp"
#include "cards.hpp"

#ifndef __PLAYER_INCLUDE
#define __PLAYER_INCLUDE

class Player {
public:
    std::vector<CardRef> builtCards;
    std::vector<CardRef> reservedCards;
    std::vector<NobleRef> nobles;
    Bank bank;
    Bank bonuses;
    unsigned int points = 0;

    bool canAfford(const Cost& cost);
    void tryPay(const Cost& cost) noexcept(false);
    void buildCard(Card& card);
    bool canHostNoble(Noble& noble);
    void hostNoble(Noble& noble);

    std::string toString() const;
};

#endif  // __PLAYER_INCLUDE
