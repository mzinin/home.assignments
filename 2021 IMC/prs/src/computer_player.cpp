#include <computer_player.hpp>

#include <limits>

namespace
{
    constexpr unsigned MIN_VALUE = 1;
    constexpr unsigned MAX_VALUE = 3;
    const std::string DEFAULT_COMPUTER_NAME = "Computer";
}

ComputerPlayer::ComputerPlayer()
    : ComputerPlayer{DEFAULT_COMPUTER_NAME}
{
}

ComputerPlayer::ComputerPlayer(std::string name)
    : mName{std::move(name)}
    , mGenerator{mRandomDevice()}
    , mDistribution{MIN_VALUE, MAX_VALUE}
{
}

Score ComputerPlayer::numberOfRounds() const
{
    return std::numeric_limits<Score>::max();
}

Throw ComputerPlayer::makeThrow() const
{
    switch (mDistribution(mGenerator))
    {
        case MIN_VALUE:
            return Throw{Figure::PAPER};

        case MAX_VALUE:
            return Throw{Figure::SCISSORS};

        default:
            return Throw{Figure::ROCK};
    }
}

const std::string& ComputerPlayer::name() const
{
    return mName;
}
