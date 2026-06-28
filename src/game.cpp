#include "game.hpp"

Game::Game(std::size_t numPlayers) noexcept(false) {
    if (numPlayers != 2 && numPlayers != 3 && numPlayers != 4) {
        throw std::invalid_argument("Illegal number of players ("
            + std::to_string(numPlayers) + " is not 2, 3, or 4)");
    }

    // === Shuffle and deal cards ===
    auto rd = std::random_device{};
    auto rng = std::default_random_engine{ rd() };
    for (auto& deck : levelDecks) {
        std::shuffle(std::begin(deck), std::end(deck), rng);
    }

    for (std::size_t i = 0; i < levelDecks.size(); i++)
    for (auto _ = 0; _ < CARD_TABLEAU_SIZE; _++) {
        levelTableaus[i].push_back(levelDecks[i].back());
        levelDecks[i].pop_back();
    }

    // === Shuffle and deal nobles ===
    std::vector<NobleRef> noblesDeck = crefs(NOBLES);
    std::shuffle(std::begin(noblesDeck), std::end(noblesDeck), rng);
    for (std::size_t _ = 0; _ < numPlayers + 1; _++) {
        noblesTableau.push_back(noblesDeck.back());
        noblesDeck.pop_back();
    }

    // === Populate the bank ===
    unsigned int gemStackSize = numPlayers == 4 ? 7 : (numPlayers == 3 ? 5 : 4);
    bank = Bank(std::array<unsigned int, NUM_CURRENCIES>{
        gemStackSize, gemStackSize, gemStackSize,
        gemStackSize, gemStackSize, INITIAL_GOLD_IN_BANK
    });

    // === Create players ===
    for (auto _ = 0; _ < numPlayers; _++) {
        players.push_back(Player());
    }
    std::uniform_int_distribution<> distr(0, numPlayers - 1);
    activePlayer = distr(rng);
}

std::string Game::toString() const {
    std::ostringstream stream;

    stream << "Game {";

    stream << "\nNobles: ";
    for (NobleRef noble : noblesTableau) {
        stream << noble.get().toString() << ", ";
    }

    stream << "\nDecks: " << levelDecks[0].size() << ", "
                          << levelDecks[1].size() << ", "
                          << levelDecks[2].size();

    stream << "\nTableaus:";
    for (std::size_t lvl = 0; lvl < 3; lvl++) {
        stream << "\n\tLevel " << lvl << ": ";
        for (CardRef card : levelTableaus[lvl]) {
            stream << card.get().toString() << ", ";
        }
    }

    stream << "\nBank: " << bank.toString();

    stream << "\nActive player: " << activePlayer;

    stream << "\nPlayers[\n";
    for (const Player& player : players) {
        stream << player.toString() << "\n";
    }
    stream << "\n]";

    stream << "\n}";

    return stream.str();
};
