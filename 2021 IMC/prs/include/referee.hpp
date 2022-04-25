#pragma once

#include <player.hpp>
#include <types.hpp>

#include <ostream>

class Referee final
{
public:
    Referee();
    Referee(std::ostream& output);

    GameResult playGame(const Player& player1, const Player& player2);

private:
    void arrangeGame(const Player& player1, const Player& player2);
    void concludeGame(const Player& player1, const Player& player2) const;
    void playRound(Score round, const Player& player1, const Player& player2);

private:
    std::ostream& mOutput;
    Score mRounds = 0;
    Score mWins1 = 0;
    Score mWins2 = 0;
};