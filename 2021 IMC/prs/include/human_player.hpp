#pragma once

#include <player.hpp>

#include <iostream>

class HumanPlayer final : public Player
{
public:
    HumanPlayer();
    HumanPlayer(std::istream& input, std::ostream& output);
    ~HumanPlayer() override = default;

    Score numberOfRounds() const override;
    Throw makeThrow() const override;
    const std::string& name() const override;

private:
    std::istream& mInput;
    std::ostream& mOutput;
};
