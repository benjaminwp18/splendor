#include <vector>
#include <functional>
#include <algorithm>
#include <random>
#include <stdexcept>
#include <string>
#include <sstream>
#include <optional>

#include "cards.hpp"
#include "player.hpp"

#ifndef __GAME_INCLUDE
#define __GAME_INCLUDE

const std::size_t CARD_TABLEAU_SIZE = 4;
const std::size_t MAX_NOBLE_TABLEAU_SIZE = 5;
const unsigned int INITIAL_GOLD_IN_BANK = 5;
const unsigned int MIN_DOUBLE_GEM_THRESHOLD = 4;
const unsigned int GAME_END_POINTS_THRESHOLD = 15;

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
    std::array<std::vector<std::optional<CardRef>>, 3> levelTableaus{
        std::vector<std::optional<CardRef>>{},
        std::vector<std::optional<CardRef>>{},
        std::vector<std::optional<CardRef>>{}
    };
    std::vector<NobleRef> noblesTableau;
    Bank bank;
    std::vector<Player> players;
    std::size_t activePlayerIdx = 0;

    Game(std::size_t numPlayers) noexcept(false);

    Player& activePlayer();
    bool canDrawCard(std::size_t deckLevel);
    const Card& tryDrawCard(std::size_t deckLevel) noexcept(false);
    bool canTakeCard(std::size_t deckLevel, std::size_t colIdx);
    const Card& tryPeekCard(std::size_t deckLevel, std::size_t colIdx) noexcept(false);
    const Card& tryTakeCard(std::size_t deckLevel, std::size_t colIdx) noexcept(false);
    bool takeGold();
    bool canTakePair(Gem gem);
    bool canTakeTriple(const std::vector<Gem>& gems);
    bool finished();

    std::string toString() const;
};

#endif  // __GAME_INCLUDE
