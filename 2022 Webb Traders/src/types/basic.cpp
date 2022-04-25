#include "types/basic.hpp"

#include <type_traits>

namespace
{
    constexpr std::string_view AMEND_CMD = "AMEND";
    constexpr std::string_view INSERT_CMD = "INSERT";
    constexpr std::string_view PULL_CMD = "PULL";

    constexpr std::string_view BUY_SIDE = "BUY";
    constexpr std::string_view SELL_SIDE = "SELL";

    constexpr char DECIMAL_POINT = '.';
    constexpr unsigned PRICE_DECIMAL_PRECISION = 4;
    constexpr std::underlying_type_t<Price> POWER10[] = {1, 10, 100, 1000, 10000};
}

// Note: no checks are done, all inputs are considered valid.

Command parseCommand(const std::string_view& str)
{
    if (str == PULL_CMD)
        return Command::PULL;
    if (str == AMEND_CMD)
        return Command::AMEND;
    return Command::INSERT;
}

Side parseSide(const std::string_view& str)
{
    return (str == BUY_SIDE) ? Side::BUY : Side::SELL;
}

OrderId parseOrderId(const std::string_view& str)
{
    OrderId result = 0;
    for (auto c : str)
    {
        result = result * 10 + (c - '0');
    }
    return result;
}

Volume parseVolume(const std::string_view& str)
{
    static_assert(std::is_same_v<OrderId, Volume>);
    return parseOrderId(str);
}

Price parsePrice(const std::string_view& str)
{
    std::underlying_type_t<Price> result = 0;
    unsigned precision = PRICE_DECIMAL_PRECISION;

    for (size_t i = 0; i < str.size(); ++i)
    {
        if (str[i] == DECIMAL_POINT)
        {
            precision -= str.size() - i - 1;
        }
        else
        {
            result = result * 10 + (str[i] - '0');
        }
    }

    return Price{result * POWER10[precision]};
}

std::ostream& operator<<(std::ostream& output, const Price& price)
{
    output << std::underlying_type_t<Price>(price) / POWER10[PRICE_DECIMAL_PRECISION];

    auto fraction = std::underlying_type_t<Price>(price) % POWER10[PRICE_DECIMAL_PRECISION];
    if (fraction != 0)
    {
        output << DECIMAL_POINT;

        auto precision = PRICE_DECIMAL_PRECISION;
        while (fraction != 0)
        {
            output << fraction / POWER10[--precision];
            fraction %= POWER10[precision];
        }
    }

    return output;
}
