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

    for (int i = 0; i < levelDecks.size(); i++)
    for (int _ = 0; _ < CARD_TABLEAU_SIZE; _++) {
        levelTableaus[i].push_back(levelDecks[i].back());
        levelDecks[i].pop_back();
    }

    // === Shuffle and deal nobles ===
    std::vector<NobleRef> noblesDeck = crefs(NOBLES);
    std::shuffle(std::begin(noblesDeck), std::end(noblesDeck), rng);
    for (int _ = 0; _ < numPlayers + 1; _++) {
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
    for (int _ = 0; _ < numPlayers; _++) {
        players.push_back(Player());
    }
    // std::uniform_int_distribution<> distr(0, numPlayers - 1);
    // activePlayerIdx = distr(rng);
}

Player& Game::activePlayer() {
    return players[activePlayerIdx];
}

bool Game::canDrawCard(std::size_t deckLevel) {
    return 0 <= deckLevel && deckLevel <= 2 && levelDecks[deckLevel].size() > 0;
}

const Card& Game::tryDrawCard(std::size_t deckLevel) noexcept(false) {
    if (!canDrawCard(deckLevel)) {
        throw NoCardsException();
    }
    const Card& card = levelDecks[deckLevel].back();
    levelDecks[deckLevel].pop_back();
    return card;
}

bool Game::canTakeCard(std::size_t deckLevel, std::size_t colIdx) {
    return 0 <= deckLevel && deckLevel <= 2
        && 0 <= colIdx && colIdx <= CARD_TABLEAU_SIZE
        && levelTableaus[deckLevel][colIdx].has_value();
}

const Card& Game::tryPeekCard(std::size_t deckLevel, std::size_t colIdx) noexcept(false) {
    if (!canTakeCard(deckLevel, colIdx)) {
        throw NoCardsException();
    }
    return levelTableaus[deckLevel][colIdx].value();
}

const Card& Game::tryTakeCard(std::size_t deckLevel, std::size_t colIdx) noexcept(false) {
    const Card& card = tryPeekCard(deckLevel, colIdx);
    levelTableaus[deckLevel][colIdx] = {};
    if (canDrawCard(deckLevel)) {
        levelTableaus[deckLevel][colIdx] = tryDrawCard(deckLevel);
    }
    return card;
}

bool Game::takeGold() {
    if (bank.getBalance(Currency::GOLD) >= 1) {
        bank.tryWithdraw(Currency::GOLD, 1);
        return true;
    }
    return false;
}

bool Game::canTakePair(Gem gem) {
    return bank.getBalance(gem) >= MIN_DOUBLE_GEM_THRESHOLD;
}

bool Game::canTakeTriple(const std::vector<Gem>& gems) noexcept(false) {
    if (gems.size() > 3) {
        throw std::invalid_argument("Triple take gems vector too long (size "
            + std::to_string(gems.size()) + " > 3)");
    }

    // Must have 3 unique gems with nonzero quantities in the bank
    for (int i = 0; i < 3 && i < gems.size(); i++) {
        if (bank.getBalance(gems[i]) == 0) {
            return false;
        }
        for (int j = 0; j < i; j++) {
            if (gems[i] == gems[j]) {
                return false;
            }
        }
    }

    return true;
}

bool Game::finished() {
    for (const Player& player : players) {
        if (player.points >= GAME_END_POINTS_THRESHOLD) {
            return true;
        }
    }
    return false;
}

std::string Game::toString() const {
    std::ostringstream stream;

    stream << "Game {";

    stream << "\nNobles: ";
    for (const Noble& noble : noblesTableau) {
        stream << noble.toString() << ", ";
    }

    stream << "\nDecks: " << levelDecks[0].size() << ", "
                          << levelDecks[1].size() << ", "
                          << levelDecks[2].size();

    stream << "\nTableaus:";
    for (int lvl = 0; lvl < 3; lvl++) {
        stream << "\n\tLevel " << lvl << ": ";
        for (const std::optional<CardRef>& optCard : levelTableaus[lvl]) {
            if (optCard.has_value()) {
                stream << optCard.value().get().toString();
            }
            stream << ", ";
        }
    }

    stream << "\nBank: " << bank.toString();

    stream << "\nActive player: " << activePlayerIdx;

    stream << "\nPlayers[\n";
    for (const Player& player : players) {
        stream << player.toString() << "\n";
    }
    stream << "\n]";

    stream << "\n}";

    return stream.str();
};
