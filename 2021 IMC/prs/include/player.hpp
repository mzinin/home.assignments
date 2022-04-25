#pragma once

#include <throw.hpp>
#include <types.hpp>

#include <ostream>
#include <string>

class Player
{
public:
    virtual ~Player() = default;
    virtual Score numberOfRounds() const = 0;
    virtual Throw makeThrow() const = 0;
    virtual const std::string& name() const = 0;
};

inline std::ostream& operator<<(std::ostream& out, const Player& player)
{
    return out << player.name();
}
