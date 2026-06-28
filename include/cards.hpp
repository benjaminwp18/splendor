#include <sstream>
#include <vector>

#include <money.hpp>

#ifndef __CONTENTS_INCLUDE
#define __CONTENTS_INCLUDE

#define __UNWRAP(...) __VA_ARGS__

#define GEN_CARDS(level, points, card0, card1, card2, card3, card4) \
    Card((level), (points), GEMS[0], std::array<unsigned int, 5>{__UNWRAP card0}), \
    Card((level), (points), GEMS[1], std::array<unsigned int, 5>{__UNWRAP card1}), \
    Card((level), (points), GEMS[2], std::array<unsigned int, 5>{__UNWRAP card2}), \
    Card((level), (points), GEMS[3], std::array<unsigned int, 5>{__UNWRAP card3}), \
    Card((level), (points), GEMS[4], std::array<unsigned int, 5>{__UNWRAP card4})

#define __CARDS_FLAT(level, points, c0, c1, c2, c3, c4) \
    GEN_CARDS(                \
        level, points,        \
        (c0, c1, c2, c3, c4), \
        (c4, c0, c1, c2, c3), \
        (c3, c4, c0, c1, c2), \
        (c2, c3, c4, c0, c1), \
        (c1, c2, c3, c4, c0)  \
    )
#define __CARDS_VA(level, points, ...) __CARDS_FLAT(level, points, __VA_ARGS__)
#define GEN_CARDS_ISO(level, points, costs) __CARDS_VA(level, points, __UNWRAP costs)

#define GEN_NOBLES_ISO(c0, c1, c2, c3, c4) \
    Noble(std::array<unsigned int, 5>{c0, c1, c2, c3, c4}), \
    Noble(std::array<unsigned int, 5>{c4, c0, c1, c2, c3}), \
    Noble(std::array<unsigned int, 5>{c3, c4, c0, c1, c2}), \
    Noble(std::array<unsigned int, 5>{c2, c3, c4, c0, c1}), \
    Noble(std::array<unsigned int, 5>{c1, c2, c3, c4, c0})

class Card {
public:
    const unsigned int level;
    const unsigned int points;
    const Gem output;
    const Cost cost;

    Card(unsigned int level, unsigned int points, Gem output, const Cost& cost) :
        level(level), points(points), output(output), cost(cost) {}

    std::string toString() const;
};

class Noble {
public:
    const Cost cost;
    const unsigned int points = 3;

    Noble(const Cost& cost) : cost(cost) {}

    std::string toString() const;
};

const std::array<std::vector<Card>, 3> CARDS{
    std::vector<Card>{
        GEN_CARDS_ISO(0, 0, (0, 1, 1, 1, 1)),
        GEN_CARDS_ISO(0, 0, (0, 2, 1, 1, 1)),
        GEN_CARDS_ISO(0, 0, (0, 2, 1, 2, 0)),
        GEN_CARDS(0, 0, (3, 0, 1, 1, 0), (1, 1, 0, 3, 0), (0, 1, 1, 0, 3), (0, 3, 0, 1, 1), (1, 0, 3, 0, 1)),
        GEN_CARDS_ISO(0, 0, (0, 0, 1, 0, 2)),
        GEN_CARDS(0, 0, (0, 0, 2, 2, 0), (0, 0, 0, 2, 2), (2, 2, 0, 0, 0), (0, 2, 2, 0, 0), (2, 0, 0, 0, 2)),
        GEN_CARDS(0, 0, (0, 0, 0, 3, 0), (0, 0, 0, 0, 3), (0, 3, 0, 0, 0), (0, 0, 3, 0, 0), (3, 0, 0, 0, 0)),
        GEN_CARDS_ISO(0, 1, (0, 4, 0, 0, 0))
    },
    std::vector<Card>{
        GEN_CARDS(1, 1, (0, 3, 2, 0, 2), (2, 0, 2, 3, 0), (3, 2, 0, 2, 0), (0, 2, 0, 2, 3), (2, 0, 3, 0, 2)),
        GEN_CARDS_ISO(1, 1, (2, 0, 0, 3, 3)),
        GEN_CARDS_ISO(1, 2, (0, 1, 2, 0, 4)),
        GEN_CARDS(1, 2, (0, 0, 3, 0, 5), (0, 3, 0, 5, 0), (0, 5, 0, 0, 3), (5, 0, 0, 3, 0), (3, 0, 5, 0, 0)),
        GEN_CARDS(1, 2, (0, 0, 0, 0, 5), (0, 5, 0, 0, 0), (5, 0, 0, 0, 0), (0, 0, 0, 5, 0), (0, 0, 5, 0, 0)),
        GEN_CARDS_ISO(1, 3, (6, 0, 0, 0, 0))
    },
    std::vector<Card>{
        GEN_CARDS_ISO(2, 3, (0, 3, 3, 3, 5)),
        GEN_CARDS_ISO(2, 4, (0, 0, 7, 0, 0)),
        GEN_CARDS_ISO(2, 4, (3, 0, 6, 0, 3)),
        GEN_CARDS_ISO(2, 5, (3, 0, 7, 0, 0))
    },
};

const std::array<Noble, 10> NOBLES{
    GEN_NOBLES_ISO(4, 0, 4, 0, 0),
    GEN_NOBLES_ISO(3, 0, 3, 3, 0)
};

#undef __UNWRAP
#undef __CARDS_FLAT
#undef __CARDS_VA

#undef GEN_CARDS
#undef GEN_CARDS_ISO
#undef GEN_NOBLES_ISO

#endif  // __CONTENTS_INCLUDE
