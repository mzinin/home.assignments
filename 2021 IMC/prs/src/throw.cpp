#include <throw.hpp>

Throw::Throw(Figure value)
    : mValue(value)
{
}

Figure Throw::value() const
{
    return mValue;
}

Outcome Throw::compare(const Throw& another) const
{
    if (mValue == another.mValue)
    {
        return Outcome::DRAW;
    }

    switch (mValue)
    {
        case Figure::PAPER:
            return another.mValue == Figure::ROCK ? Outcome::WIN : Outcome::DEFEAT;

        case Figure::ROCK:
            return another.mValue == Figure::SCISSORS ? Outcome::WIN : Outcome::DEFEAT;

        case Figure::SCISSORS:
        default:
            return another.mValue == Figure::PAPER ? Outcome::WIN : Outcome::DEFEAT;
    }
}

std::ostream& operator<<(std::ostream& out, const Throw& thrw)
{
    switch (thrw.mValue)
    {
        case Figure::PAPER:
            out << "PAPER";
            break;

        case Figure::ROCK:
            out << "ROCK";
            break;

        case Figure::SCISSORS:
            out << "SCISSORS";
            break;
    }
    return out;
}
