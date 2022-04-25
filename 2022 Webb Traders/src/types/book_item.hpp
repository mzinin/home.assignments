#pragma once

#include "types/basic.hpp"

#include <optional>
#include <string>

// Represents an item of an order book state.
struct BookItem final
{
    std::optional<Price> buyPrice;
    std::optional<Volume> buyVolume;
    std::optional<Price> sellPrice;
    std::optional<Volume> sellVolume;

    BookItem() = default;

    // Set all buy-side data.
    void setBuy(const Price price, const Volume volume);

    // Set all sell-side data.
    void setSell(const Price price, const Volume volume);

    std::string toString() const;
};
