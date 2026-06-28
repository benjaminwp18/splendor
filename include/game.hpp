#include <vector>
#include <functional>

#include "cards.hpp"
#include "player.hpp"

#ifndef __GAME_INCLUDE
#define __GAME_INCLUDE

const std::size_t NUM_PLAYERS = 5;

class Game {
    std::array<std::vector<std::reference_wrapper<Card>>, 3> levelDecks;
    std::array<std::array<std::reference_wrapper<Card>, NUM_PLAYERS>, 3> levelTableaus;
    std::array<std::reference_wrapper<Noble>, NUM_PLAYERS + 1> nobleTableau;
    Bank bank;
    std::array<Player, NUM_PLAYERS> players;
};

#endif  // __GAME_INCLUDE