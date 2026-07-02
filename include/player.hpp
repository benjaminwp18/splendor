#include <vector>
#include <functional>
#include <assert.h>
#include <numeric>
#include <sstream>

#include "money.hpp"
#include "cards.hpp"

#ifndef __PLAYER_INCLUDE
#define __PLAYER_INCLUDE

const std::size_t MAX_RESERVATIONS = 3;
const unsigned int MAX_BANK_WEALTH = 10;

class Player {
private:
    bool canAfford(const Cost& cost) const;
    void tryPay(const Cost& cost) noexcept(false);

public:
    // TODO: the mutable views of these fields could probably be private?
    std::vector<CardRef> builtCards;
    std::vector<CardRef> reservedCards;
    std::vector<NobleRef> nobles;
    Bank bank;
    Bank bonuses;
    unsigned int points = 0;

    bool canBuildCard(const Card& card) const;
    void tryBuildCard(const Card& card) noexcept(false);
    bool canHostNoble(const Noble& noble) const;
    void hostNoble(const Noble& noble);
    bool canReserve() const;
    void tryReserve(const Card& card) noexcept(false);
    bool canBuildReserved(std::size_t reserveIdx) const;
    void tryBuildReserved(std::size_t reserveIdx) noexcept(false);
    bool isTooRich() const;

    std::string toString() const;
};

#endif  // __PLAYER_INCLUDE
