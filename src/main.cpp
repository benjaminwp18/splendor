#include "main.hpp"

void clearScreen() {
    #ifdef _WIN32
        std::system("cls");
    #else
        std::system("clear");
    #endif
}

std::string gemCost(const Cost& cost, Gem gem) {
    if (cost.forGem(gem) > 0) {
        return std::to_string(cost.forGem(gem))
               + currencyToString(gem)[0];
    }
    else {
        return "  ";
    }
}

const std::size_t NUM_STREAMS = 7;

void flush(std::array<std::ostringstream, NUM_STREAMS>& streams) {
    for (auto& stream : streams) {
        std::cout << " " << stream.str() << "\n";
        stream.str("");
    }
}

unsigned int oneDigit(const Bank& bank, Currency currency) {
    return std::min(9U, bank.getBalance(currency));
}

void renderCard(std::array<std::ostringstream, NUM_STREAMS>& streams,
        const std::optional<CardRef>& optCard) {
    if (optCard.has_value()) {
        const Card& card = optCard.value();
        streams[0] << "           ";
        streams[1] << "+--------+ ";
        streams[2] << "| " << card.points << "    "
                    << currencyToString(card.output)[0] << " | ";
        streams[3] << "|   " << gemCost(card.cost, Gem::DIAMOND) << "   | ";
        streams[4] << "| " << gemCost(card.cost, Gem::SAPPHIRE) << "  "
                            << gemCost(card.cost, Gem::RUBY) << " | ";
        streams[5] << "| " << gemCost(card.cost, Gem::EMERALD) << "  "
                            << gemCost(card.cost, Gem::ONYX) << " | ";
        streams[6] << "+--------+ ";
    }
    else {
        for (auto& stream : streams) {
            stream << "           ";
        }
    }
}

void renderDeck(std::array<std::ostringstream, NUM_STREAMS>& streams,
                std::size_t deckSize) {
    streams[0] << "._________  ";
    streams[1] << "|+--------+ ";
    streams[2] << "||        | ";
    streams[3] << "||   " << std::setw(2) << std::setfill('0') << deckSize << "   | ";
    streams[4] << "||        | ";
    streams[5] << "||        | ";
    streams[6] << " +--------+ ";
}

void renderPlayer(std::array<std::ostringstream, NUM_STREAMS>& streams,
                  const Player& player, std::size_t playerIdx, bool isCurrentPlayer,
                  bool isUser) {
    streams[0] << "   ";
    streams[1] << "   ";
    streams[2] << "   ";
    streams[3] << "   ";
    streams[4] << "   ";
    streams[5] << "   ";
    streams[6] << "   ";

    streams[0] << "     ";
    for (int i = 1; i < streams.size() - 1; i++) {
        streams[i] << (isCurrentPlayer ? ">" : " ")
                   << playerIdx
                   << (isUser ? "U" : std::to_string(playerIdx))
                   << "  ";
    }
    streams[6] << "     ";

    streams[0] << "._________               ";
    streams[1] << "|+--------+   .------.   ";
    streams[2] << "|| " << std::setw(2) << std::setfill('0') << player.points
               << "     |  / " << oneDigit(player.bank, Currency::DIAMOND)
               << "D  " << oneDigit(player.bank, Currency::GOLD)
               << "G \\  ";
    streams[3] << "||   " << oneDigit(player.bonuses, Currency::DIAMOND)
               << "D   |  | " << oneDigit(player.bank, Currency::SAPPHIRE)
               << "S  " << oneDigit(player.bank, Currency::RUBY)
               << "R |  ";
    streams[4] << "|| " << oneDigit(player.bonuses, Currency::SAPPHIRE)
               << "S  " << oneDigit(player.bonuses, Currency::RUBY)
               << "R |  \\ " << oneDigit(player.bank, Currency::EMERALD)
               << "E  " << oneDigit(player.bank, Currency::ONYX)
               << "O /  ";
    streams[5] << "|| " << oneDigit(player.bonuses, Currency::EMERALD)
               << "E  " << oneDigit(player.bonuses, Currency::ONYX)
               << "O |   '------'   ";
    streams[6] << " +--------+              ";

    if (isUser) {
        if (player.reservedCards.size() == 0) {
            streams[3] << "(no reserved cards)";
        }
        else {
            for (auto& stream : streams) {
                stream << ' ';
            }
            for (const Card& card : player.reservedCards) {
                renderCard(streams, card);
            }
        }
    }
    else {
        renderDeck(streams, player.reservedCards.size());
    }
}

void renderGame(std::array<std::ostringstream, 7>& streams, const Game& game) {
    clearScreen();

    streams[0] << "   ";
    streams[1] << "   ";
    streams[2] << "   ";
    streams[3] << "   ";
    streams[4] << "   ";
    streams[5] << "   ";
    streams[6] << "   ";

    //                +--------+ +--------+ +--------+ +--------+ +--------+
    std::cout << "        CC         VV         BB         NN         MM      "
              << "                                     JJ         KK         LL     \n";
    for (const Noble& noble : game.noblesTableau) {
        streams[0] << "           ";
        streams[1] << "#========# ";
        streams[2] << "{ " << noble.points << "      } ";
        streams[3] << "{   " << gemCost(noble.cost, Gem::DIAMOND) << "   } ";
        streams[4] << "{ " << gemCost(noble.cost, Gem::SAPPHIRE) << "  "
                            << gemCost(noble.cost, Gem::RUBY) << " } ";
        streams[5] << "{ " << gemCost(noble.cost, Gem::EMERALD) << "  "
                            << gemCost(noble.cost, Gem::ONYX) << " } ";
        streams[6] << "#========# ";
    }

    for (int _ = game.noblesTableau.size(); _ < MAX_NOBLE_TABLEAU_SIZE; _++) {
        for (auto& stream : streams) {
            stream << "           ";
        }
    }

    renderPlayer(streams, game.players[0], 0,
                    game.activePlayerIdx == 0, game.activePlayerIdx == 0);

    flush(streams);

    int lvl = -1;
    for (int i = 0; i < game.levelTableaus.size(); i++) {
        lvl = game.levelTableaus.size() - 1 - i;

        streams[0] << "  ";
        streams[1] << "  ";
        streams[2] << "  ";
        streams[3] << lvl + 1 << " ";
        streams[4] << lvl + 1 << " ";
        streams[5] << "  ";
        streams[6] << "  ";

        renderDeck(streams, game.levelDecks[lvl].size());

        for (const std::optional<CardRef>& optCard : game.levelTableaus[lvl]) {
            renderCard(streams, optCard);
        }

        if (i + 1 < game.players.size()) {
            renderPlayer(streams, game.players[i + 1], i + 1,
                        game.activePlayerIdx == i + 1, game.activePlayerIdx == i + 1);
        }

        flush(streams);
    }

    std::cout << "\n";

    streams[0] << "  .----------.   ";
    streams[1] << " /  " << game.bank.getBalance(Currency::DIAMOND) << "D    "
               << game.bank.getBalance(Currency::GOLD) << "G  \\  ";
    streams[2] << "|              | ";
    streams[3] << "|   " << game.bank.getBalance(Currency::SAPPHIRE) << "S    "
               << game.bank.getBalance(Currency::RUBY) << "R   | ";
    streams[4] << "|              | ";
    streams[5] << " \\  " << game.bank.getBalance(Currency::EMERALD) << "E    "
               << game.bank.getBalance(Currency::ONYX) << "O  /  ";
    streams[6] << "  '----------'   ";

    streams[0] << " Actions:";
    streams[1] << "     - Take 3 unique gems: Enter the 1-letter abbreviations of 3 (or fewer) unique gems (e.g. \"ros\" for ruby, onyx, and sapphire).";
    streams[2] << "     - Take 2 of the same gem: Enter the 1-letter abbreviation of the same gem twice (e.g. \"rr\" for two rubies). There must be >=4 of this gem available.";
    streams[3] << "     - Build a card from the table: Enter the row then the column of the card to build (e.g. \"2v\" for the farthest left face-up card in the Level 1 row).";
    streams[4] << "     - Reserve a card: Enter \"g\" and the coordinates of the card to reserve (e.g. \"g3m\") or of the deck to blind reserve from (e.g. \"g3c\").";
    streams[5] << "     - Build a card from your reserved hand: Enter \"g\" and the index of the card to build (e.g. \"gl\" for the last card in a 3-card hand).";

    flush(streams);
}

// const std::array<char, NUM_CURRENCIES> GEM_CHARS{'d', 'e', 'o', 's', 'r'};
#define ACTION_ENTRY(uppercase, lowercase) {lowercase, lowercase}, {uppercase, lowercase}
namespace ActionChars {
    const std::unordered_map<char, Gem> GEMS = {
        {'d', Gem::DIAMOND},
        {'e', Gem::EMERALD},
        {'o', Gem::ONYX},
        {'s', Gem::SAPPHIRE},
        {'r', Gem::RUBY},
    };
    const std::unordered_map<char, Currency> CURRENCY = {
        {'d', Currency::DIAMOND},
        {'e', Currency::EMERALD},
        {'o', Currency::ONYX},
        {'s', Currency::SAPPHIRE},
        {'r', Currency::RUBY},
        {'g', Currency::GOLD}
    };
    const std::unordered_map<char, std::size_t> HAND_IDXS = {
        {'j', 0},
        {'k', 1},
        {'l', 2},
    };
    const std::unordered_map<char, std::size_t> LEVEL_IDXS = {
        {'1', 0},
        {'2', 1},
        {'3', 2},
    };
    const std::unordered_map<char, int> COLUMN_IDXS = {
        {'c', -1},
        {'v', 0},
        {'b', 1},
        {'n', 2},
        {'m', 3},
    };
    const std::unordered_map<char, char> ALL = {
        ACTION_ENTRY('D', 'd'),
        ACTION_ENTRY('E', 'e'),
        ACTION_ENTRY('O', 'o'),
        ACTION_ENTRY('S', 's'),
        ACTION_ENTRY('R', 'r'),
        ACTION_ENTRY('G', 'g'),

        ACTION_ENTRY('C', 'c'),
        ACTION_ENTRY('V', 'v'),
        ACTION_ENTRY('B', 'b'),
        ACTION_ENTRY('N', 'n'),
        ACTION_ENTRY('M', 'm'),
        ACTION_ENTRY('J', 'j'),
        ACTION_ENTRY('K', 'k'),
        ACTION_ENTRY('L', 'l'),
        ACTION_ENTRY('N', 'n'),

        {'1', '1'},
        {'2', '2'},
        {'3', '3'},
    };
}

void readAction(std::vector<char>& action) {
    std::string input;
    std::getline(std::cin, input);

    action.clear();
    for (char c : input) {
        auto actionCharEntry = ActionChars::ALL.find(c);
        if (actionCharEntry != ActionChars::ALL.end()) {
            action.push_back(actionCharEntry->second);
        }
    }
}

std::optional<std::pair<std::size_t, int>> parseCoords(
    std::vector<char>& action,
    std::size_t startIdx
) {
    std::optional<int> colIdx = {};
    std::optional<std::size_t> levelIdx = {};

    for (int i = startIdx; i < startIdx + 2 && i < action.size(); i++) {
        auto colIdxEntry = ActionChars::COLUMN_IDXS.find(action[i]);
        if (colIdxEntry != ActionChars::COLUMN_IDXS.end()) {
            colIdx = colIdxEntry->second;
        }

        auto lvlIdxEntry = ActionChars::LEVEL_IDXS.find(action[i]);
        if (lvlIdxEntry != ActionChars::LEVEL_IDXS.end()) {
            levelIdx = lvlIdxEntry->second;
        }
    }

    if (!colIdx.has_value() || !levelIdx.has_value()) {
        std::cout << "You must have two coordinate characters (one 123 & one cvbnm) in a "
                  << "card reserve or build. ";
        return {};
    }

    return {{levelIdx.value(), colIdx.value()}};
}

void limitPlayerWealth(Game& game) {
    std::vector<char> action;
    while (game.activePlayer().isTooRich()) {
        std::cout << "You have >10 currency. Discard currency until "
                  << "you have <=10. Enter currency chars (deosrg): ";
        readAction(action);

        for (char c : action) {
            auto currencyEntry = ActionChars::CURRENCY.find(c);
            if (currencyEntry == ActionChars::CURRENCY.end()) {
                continue;
            }

            if (game.activePlayer().bank.getBalance(currencyEntry->second) >= 1) {
                game.activePlayer().bank.tryWithdraw(currencyEntry->second, 1);
            }
            else {
                std::cout << "You can't discard any more "
                          << currencyToString(currencyEntry->second) << ".\n";
            }
        }
    }
}

int main() {
    std::size_t numPlayers = 3;
    Game game(numPlayers);

    std::array<std::ostringstream, 7> streams;
    std::string input;
    std::vector<char> action;

    // bool actionIsGemDraw = false;
    // bool actionIsReserve = false;
    // std::vector<
    // std::unordered_map<Gem, unsigned int> gemCounts = {};
    // std::transform(
    //     ActionChars::GEMS.begin(),
    //     ActionChars::GEMS.end(),
    //     gemCounts.begin(),
    //     [](const std::pair<char, Gem>& pair) {
    //         return std::pair<Gem, unsigned int>{pair.second, 0};
    //     }
    // );

    while (!game.finished()) {
        for (
            game.activePlayerIdx = 0;
            game.activePlayerIdx < numPlayers;
            game.activePlayerIdx++
        ) {
            renderGame(streams, game);

            std::cout << "\n";

            while (true) {
                std::cout << "Enter action: ";
                readAction(action);

                if (action.size() == 0) {
                    // TODO: It might theoretically be possible to force a skip
                    std::cout << "You can't skip your turn. ";
                    continue;
                }

                if (action[0] == 'g') {
                    // Reserve or build a reserved card
                    if (action.size() < 2) {
                        std::cout << "You must specify which card to reserve or build. ";
                        continue;
                    }

                    auto handIdxEntry = ActionChars::HAND_IDXS.find(action[1]);
                    if (handIdxEntry != ActionChars::HAND_IDXS.end()) {
                        // Build a reserved card
                        if (!game.activePlayer().canBuildReserved(handIdxEntry->second)) {
                            std::cout << "You cannot build reserved card "
                                      << (handIdxEntry->second + 1) << "/3. ";
                            continue;
                        }

                        game.activePlayer().tryBuildReserved(handIdxEntry->second);
                        break;
                    }

                    // Reserve a card
                    if (!game.activePlayer().canReserve()) {
                        std::cout << "Your reservation hand is full. ";
                        continue;
                    }

                    auto optCoords = parseCoords(action, 1);
                    if (!optCoords.has_value()) {
                        continue;
                    }
                    auto coords = optCoords.value();

                    if (coords.second < 0) {
                        // Blind reserve (from a deck)
                        if (!game.canDrawCard(coords.first)) {
                            std::cout << "Deck #" << (coords.first + 1) << " is empty. ";
                            continue;
                        }

                        game.activePlayer().tryReserve(game.tryDrawCard(coords.first));
                    }
                    else {
                        // Reserve from tableau
                        if (!game.canTakeCard(coords.first, coords.second)) {
                            std::cout << "There is no card at column "
                                      << (coords.second + 1) << "/"
                                      << CARD_TABLEAU_SIZE << " in level "
                                      << (coords.first + 1) << ". ";
                            continue;
                        }

                        game.activePlayer().tryReserve(
                            game.tryTakeCard(coords.first, coords.second));
                    }

                    if (game.takeGold()) {
                        game.activePlayer().bank.deposit(Currency::GOLD, 1);
                    }

                    limitPlayerWealth(game);
                    break;
                }
                else if (ActionChars::GEMS.count(action[0]) > 0) {
                    // TODO: Take gems


                }
                else {
                    // Build card
                    auto optCoords = parseCoords(action, 0);
                    if (!optCoords.has_value()) {
                        continue;
                    }

                    auto coords = optCoords.value();

                    if (coords.second < 0) {
                        std::cout << "You cannot build a card blind (from a deck). ";
                        continue;
                    }

                    if (!game.canTakeCard(coords.first, coords.second)) {
                        std::cout << "There is no card at column "
                                  << (coords.second + 1) << "/"
                                  << CARD_TABLEAU_SIZE << " in level "
                                  << coords.first << ". ";
                        continue;
                    }

                    if (!game.activePlayer().canBuildCard(
                        game.tryPeekCard(coords.first, coords.second)
                    )) {
                        std::cout << "You cannot afford the card at column "
                                  << (coords.second + 1) << "/"
                                  << CARD_TABLEAU_SIZE << " in level "
                                  << (coords.first + 1) << ". ";
                        continue;
                    }

                    game.activePlayer()
                        .tryBuildCard(game.tryTakeCard(coords.first, coords.second));
                }
            }
        }
    }
}
