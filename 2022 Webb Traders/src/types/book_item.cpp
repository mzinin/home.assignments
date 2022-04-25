#include "types/book_item.hpp"

#include <sstream>

namespace
{
    template <typename T>
    std::ostream& operator<<(std::ostream& output, const std::optional<T>& opt)
    {
        if (opt)
        {
            output << *opt;
        }
        return output;
    }
}

void BookItem::setBuy(const Price price, const Volume volume)
{
    buyPrice = price;
    buyVolume = volume;
}

void BookItem::setSell(const Price price, const Volume volume)
{
    sellPrice = price;
    sellVolume = volume;
}

std::string BookItem::toString() const
{
    std::ostringstream buffer;
    buffer << buyPrice << DELIMITER
           << buyVolume << DELIMITER
           << sellPrice << DELIMITER
           << sellVolume;
    return buffer.str();
}
