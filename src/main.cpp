#include "main.hpp"

int main() {
    for (std::size_t i = 0; i < CARDS.size(); i++) {
        std::cout << "Level " << i << ":\n";
        for (auto card : CARDS[i]) {
            std::cout << " - " << card.toString() << "\n";
        }
        std::cout << "\n";
    }

    for (auto noble : NOBLES) {
        std::cout << noble.toString() << "\n";
    }
}
