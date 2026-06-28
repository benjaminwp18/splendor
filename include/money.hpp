#include <cstddef>
#include <array>
#include <string>
#include <assert.h>
#include <sstream>

enum class Gem {
    DIAMOND  = 0,  // WHITE
    EMERALD  = 1,  // GREEN
    ONYX     = 2,  // BLACK
    SAPPHIRE = 3,  // BLUE
    RUBY     = 4   // RED
};

enum class Currency {
    DIAMOND  = 0,  // WHITE
    EMERALD  = 1,  // GREEN
    ONYX     = 2,  // BLACK
    SAPPHIRE = 3,  // BLUE
    RUBY     = 4,  // RED
    GOLD     = 5   // WILD
};

class GemWrapper {
private:
    Gem gem;

public:
    constexpr GemWrapper(Gem gem) : gem(gem) {}

    constexpr operator Gem() const { return gem; }
    constexpr operator Currency() const { return gemToCurrency(gem); }
    explicit operator bool() const = delete;

    constexpr std::size_t asIndex() { return static_cast<std::size_t>(gem); }
};

class CurrencyWrapper {
private:
    Currency currency;

public:
    constexpr CurrencyWrapper(Currency currency) : currency(currency) {}
    constexpr CurrencyWrapper(Gem gem) : currency(gemToCurrency(gem)) {}

    constexpr operator Currency() const { return currency; }
    explicit operator bool() const = delete;

    constexpr std::size_t asIndex() { return static_cast<std::size_t>(currency); }
};

const std::size_t NUM_GEMS = 5;
const std::size_t NUM_CURRENCIES = NUM_GEMS + 1;

const std::array<Gem, NUM_GEMS> GEMS{Gem::DIAMOND, Gem::EMERALD, Gem::ONYX, Gem::SAPPHIRE, Gem::RUBY};
const std::array<Currency, NUM_CURRENCIES> CURRENCIES{Currency::DIAMOND, Currency::EMERALD, Currency::ONYX, Currency::SAPPHIRE, Currency::RUBY, Currency::GOLD};
const std::array<std::string, NUM_CURRENCIES> CURRENCY_TO_STRING{"DIAMOND", "EMERALD", "ONYX", "SAPPHIRE", "RUBY", "GOLD"};

Currency gemToCurrency(Gem gem) {
    return CURRENCIES[static_cast<std::size_t>(gem)];
}

class Cost {
private:
    const std::array<unsigned int, NUM_GEMS> costs;

public:
    Cost(const std::array<unsigned int, NUM_GEMS> &costs) :
        costs(costs) {}

    unsigned int forGem(GemWrapper gem) const;

    std::string toString() const;
};

class Bank {
private:
    std::array<unsigned int, NUM_CURRENCIES> currencies;

public:
    Bank(const std::array<unsigned int, NUM_CURRENCIES>& currencies) :
        currencies(currencies) {}
    Bank() : currencies(std::array<unsigned int, NUM_CURRENCIES>()) {}

    unsigned int getBalance(CurrencyWrapper currency);
    unsigned int deposit(CurrencyWrapper currency, unsigned int amount);
    bool tryWithdraw(CurrencyWrapper currency, unsigned int amount);
    bool tryWithdraw(const Cost& cost);
    bool canAfford(const Cost& cost);
};
