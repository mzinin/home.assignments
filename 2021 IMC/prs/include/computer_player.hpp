#pragma once

#include <player.hpp>

#include <random>

class ComputerPlayer final : public Player
{
public:
    ComputerPlayer();
    ComputerPlayer(std::string name);
    ~ComputerPlayer() override = default;

    Score numberOfRounds() const override;
    Throw makeThrow() const override;
    const std::string& name() const override;

private:
    std::string mName;
    std::random_device mRandomDevice;
    mutable std::mt19937 mGenerator;
    mutable std::uniform_int_distribution<unsigned> mDistribution;
};
