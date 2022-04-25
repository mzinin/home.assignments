#pragma once

#include <cstdint>
#include <ostream>
#include <type_traits>

using Score = uint8_t;

// If Score is one of chars, print it as a number
inline std::ostream& operator<<(std::ostream& out, unsigned char score)
{
    return out << static_cast<int>(score);
}
inline std::ostream& operator<<(std::ostream& out, signed char score)
{
    return out << static_cast<int>(score);
}
inline std::ostream& operator<<(std::ostream& out, char score)
{
    return out << static_cast<int>(score);
}

struct GameResult
{
    Score numberOfRounds = 0;
    Score firstPlayerWins = 0;
    Score secondPlayerWins = 0;
    Score draws = 0;
};

enum class Figure : uint8_t
{
    PAPER,
    ROCK,
    SCISSORS
};

enum class Outcome : uint8_t
{
    WIN,
    DRAW,
    DEFEAT
};
