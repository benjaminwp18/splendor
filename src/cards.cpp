#include "cards.hpp"

std::string Card::toString() const {
    return (std::ostringstream() << "Card: l" << level << " p" << points << ": " <<
            cost.toString() << " -> " <<
            CURRENCY_TO_STRING[static_cast<std::size_t>(output)]).str();
}

std::string Noble::toString() const {
    return (std::ostringstream() <<
            "Noble: p" << points << ": " << cost.toString()).str();
}
