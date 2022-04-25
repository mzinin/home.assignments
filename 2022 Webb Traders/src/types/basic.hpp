#pragma once

#include <cstdint>
#include <ostream>
#include <string_view>

// Field delimiter in text data.
inline constexpr char DELIMITER = ',';

// One of 3 suported commands.
enum class Command : uint8_t
{
    AMEND,
    INSERT,
    PULL
};

// One of 2 supported trade sides.
enum class Side : uint8_t
{
    BUY,
    SELL
};

// Asset's symbol.
// TODO: comsider converting to int64_t if length of symbol <= 8.
using Symbol = std::string_view;

// Order id, expected to be unique through all events.
using OrderId = uint32_t;

// No limits for volume is known, let's suggest uint32_t is enough.
using Volume = uint32_t;

// Store price as integer value to avoid all floating point related problems.
// Price's precision is limited by 4 decimal digits, so uint32_t is enough.
enum class Price : uint32_t {};

// Parsing functions to get values from a string.
Command parseCommand(const std::string_view& str);
Side parseSide(const std::string_view& str);
OrderId parseOrderId(const std::string_view& str);
Volume parseVolume(const std::string_view& str);
Price parsePrice(const std::string_view& str);

// And a custom operator for a price.
std::ostream& operator<<(std::ostream& output, const Price& price);
