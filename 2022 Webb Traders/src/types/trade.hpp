#pragma once

#include <types/basic.hpp>

#include <string>

// Represemnts an actual trade.
struct Trade final
{
    Price price;
    Volume volume;
    OrderId aggressiveOrderId;
    OrderId passiveOrderId;
    Symbol symbol;

    std::string toString() const;
};
