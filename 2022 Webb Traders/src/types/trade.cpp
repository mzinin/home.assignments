#include "types/trade.hpp"

#include <sstream>

std::string Trade::toString() const
{
    std::ostringstream buffer;
    buffer << symbol << DELIMITER
           << price << DELIMITER
           << volume << DELIMITER
           << aggressiveOrderId << DELIMITER
           << passiveOrderId;
    return buffer.str();
}
