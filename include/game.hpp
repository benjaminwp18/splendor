#include <vector>
#include <functional>
#include <algorithm>
#include <random>
#include <stdexcept>
#include <string>
#include <sstream>

#include "cards.hpp"
#include "player.hpp"

#ifndef __GAME_INCLUDE
#define __GAME_INCLUDE

const std::size_t CARD_TABLEAU_SIZE = 4;
const unsigned int INITIAL_GOLD_IN_BANK = 5;

template<class T>
static std::vector<std::reference_wrapper<const T>> crefs(const std::vector<T>& src) {
    std::vector<std::reference_wrapper<const T>> dst;
    dst.reserve(src.size());
    for (const auto& x : src) {
        dst.push_back(std::cref(x));
    }
    return dst;
}

class Game {
public:
    std::array<std::vector<CardRef>, 3> levelDecks{
        crefs(CARDS[0]),
        crefs(CARDS[1]),
        crefs(CARDS[2])
    };
    std::array<std::vector<CardRef>, 3> levelTableaus{
        std::vector<CardRef>{},
        std::vector<CardRef>{},
        std::vector<CardRef>{}
    };
    std::vector<NobleRef> noblesTableau;
    Bank bank;
    std::vector<Player> players;
    std::size_t activePlayer;

    Game(std::size_t numPlayers) noexcept(false);

    std::string toString() const;
};

#endif  // __GAME_INCLUDE
