#include "contents.hpp"

unsigned int Cost::getCost(Gem gem) const {
    return costs[gem];
}

std::string Cost::toString() const {
    return (std::ostringstream() <<
            costs[0] << costs[1] << costs[2] << costs[3] << costs[4]).str();
}

std::string Card::toString() const {
    return (std::ostringstream() << "Card: l" << level << " p" << points << ": " <<
            cost.toString() << " -> " << GEM_TO_STRING[output]).str();
}

std::string Noble::toString() const {
    return (std::ostringstream() <<
            "Noble: p" << points << ": " << cost.toString()).str();
}
