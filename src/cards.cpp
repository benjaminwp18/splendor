#include "cards.hpp"

std::string Card::toString() const {
    return (std::ostringstream() << "CardL" << level << "("
            << cost.toString() << " -> "
            << currencyToString(output) << ", p" << points << ")").str();
}

std::string Noble::toString() const {
    return (std::ostringstream() << "Noble(" << cost.toString()
            << " -> p" << points << ")").str();
}
