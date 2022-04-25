#include <referee.hpp>

#include <iostream>
#include <stdexcept>

Referee::Referee()
    : Referee(std::cout)
{
}

Referee::Referee(std::ostream& output)
    : mOutput(output)
{
}

GameResult Referee::playGame(const Player& player1, const Player& player2)
{
    arrangeGame(player1, player2);

    for (Score round = 0; round < mRounds; ++round)
    {
        playRound(round + 1, player1, player2);
    }

    concludeGame(player1, player2);
    return GameResult{mRounds, mWins1, mWins2, static_cast<Score>(mRounds - mWins1 - mWins2)};
}

void Referee::arrangeGame(const Player& player1, const Player& player2)
{
    if (&player1 == &player2)
    {
        throw std::runtime_error("Nice try, but a player cannot play against themselves");
    }

    mRounds = std::min(player1.numberOfRounds(), player2.numberOfRounds());
    mWins1 = 0;
    mWins2 = 0;

    mOutput << "Starting a new game of " << mRounds << " rounds" << std::endl;
}

void Referee::concludeGame(const Player& player1, const Player& player2) const
{
    mOutput << "\nGame is finished, " << mRounds << " rounds were played:\n"
            << player1 << " won " << mWins1 << " round(s)\n"
            << player2 << " won " << mWins2 << " round(s)\n"
            << (mRounds - mWins1 - mWins2) << " round(s) ended in a draw\n";

    if (mWins1 > mWins2)
    {
        mOutput << player1 << " is the winner!";
    }
    else if (mWins1 < mWins2)
    {
        mOutput << player2 << " is the winner!";
    }
    else
    {
        mOutput << "No winner in this game, it's a draw.";
    }
    mOutput << std::endl;
}

void Referee::playRound(Score round, const Player& player1, const Player& player2)
{
    mOutput << "\nRound " << round << ":" << std::endl;

    const auto throw1 = player1.makeThrow();
    const auto throw2 = player2.makeThrow();

    mOutput << player1 << "'s throw: " << throw1 << "\n";
    mOutput << player2 << "'s throw: " << throw2 << "\n";

    switch (throw1.compare(throw2))
    {
        case Outcome::WIN:
            ++mWins1;
            mOutput << player1 << " won round " << round;
            break;

        case Outcome::DEFEAT:
            ++mWins2;
            mOutput << player2 << " won round " << round;
            break;

        case Outcome::DRAW:
            mOutput << "A draw in the round " << round;
            break;
    }
    mOutput << std::endl;
}