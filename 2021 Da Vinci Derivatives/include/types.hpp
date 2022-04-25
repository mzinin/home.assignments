#pragma once

#include <cstdint>


using Timestamp = uint64_t;

using Symbol = uint64_t;

using OrderID = uint32_t;

using Volume = uint32_t;

// Price in tenths of units (i.e. 13.7 -> 137).
using Price = int32_t;

enum class Operation : uint8_t
{
    INSERT,
    CANCEL,
    AMEND
};

enum class Side : uint8_t
{
    BUY,
    SELL
};

struct Record
{
    Timestamp ts;
    Symbol symbol;
    OrderID orderID;
    Volume volume;
    Price price;
    Operation operation;
    Side side;
};
