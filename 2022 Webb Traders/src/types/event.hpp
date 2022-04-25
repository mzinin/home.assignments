#pragma once

#include "types/basic.hpp"

#include <string>

// Represents one order event.
struct Event final
{
    OrderId orderId;
    Price price;
    Volume volume;
    Command command;
    Side side;
    Symbol symbol;

    // Constructs event from a string.
    // String lifetime must exceeds the one of the event.
    Event(const std::string& str);
};
