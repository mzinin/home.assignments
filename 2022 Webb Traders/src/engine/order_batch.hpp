#pragma once

#include "types/basic.hpp"

#include <list>
#include <unordered_map>

// A set of orders with the same price.
class OrderBatch final
{
public:
    struct Order final
    {
        OrderId id;
        Volume volume;
    };

public:
    OrderBatch() = default;

    // Add a new order to the batch.
    // @return true is order was added, false otherwise
    bool add(const OrderId orderId, const Volume volume);

    // Erase an existing order from the batch.
    // @return true is order was erased, false otherwise
    bool erase(const OrderId orderId);

    // Update volume of an existing order by order id.
    // @return true is order was updated, false otherwise
    bool updateVolume(const OrderId orderId, const Volume newVolume);

    // Update volume of an existing order by reference to the order.
    // Must be used only for descreasing order's volume.
    // @return true is order was updated, false otherwise
    bool updateVolume(Order& order, const Volume newVolume);

    // Get the top (chonologically) order of the batch.
    Order& topOrder();

    // Check if the batch is empty.
    bool empty() const;

    // Get total volume of all order in the batch.
    Volume totalVolume() const;

private:
    Volume totalVolume_ = 0;
    std::list<Order> orders_;
    std::unordered_map<OrderId, std::list<Order>::iterator> hash_;
};
