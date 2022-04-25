#pragma once

#include <types.hpp>

#include <ostream>

class Throw final
{
public:
    Throw(Figure value);
    Figure value() const;
    Outcome compare(const Throw& another) const;

    friend std::ostream& operator<<(std::ostream& out, const Throw& thrw);

private:
    Figure mValue;
};
