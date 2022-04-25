#include "engine/order_batch.hpp"

#include <cassert>

bool OrderBatch::add(const OrderId orderId, const Volume volume)
{
    auto hashIt = hash_.find(orderId);
    if (hashIt != hash_.end())
    {
        return false;
    }

    totalVolume_ += volume;

    orders_.emplace_back(Order{orderId, volume});
    hash_[orderId] = --orders_.end();

    return true;
}

bool OrderBatch::erase(const OrderId orderId)
{
    auto hashIt = hash_.find(orderId);
    if (hashIt == hash_.end())
    {
        return false;
    }

    auto& orderIt = hashIt->second;
    totalVolume_ -= orderIt->volume;

    orders_.erase(orderIt);
    hash_.erase(hashIt);

    return true;
}

bool OrderBatch::updateVolume(const OrderId orderId, const Volume newVolume)
{
    auto hashIt = hash_.find(orderId);
    if (hashIt == hash_.end())
    {
        return false;
    }

    auto& orderIt = hashIt->second;
    auto& order = *orderIt;

    if (order.volume >= newVolume)
    {
        // if volume is descreased, just update volume
        return updateVolume(order, newVolume);
    }
    else
    {
        // otherwise, change order's priority
        totalVolume_ += newVolume - order.volume;
        orders_.erase(orderIt);
        orders_.emplace_back(Order{orderId, newVolume});
        hashIt->second = --orders_.end();
        return true;
    }
}

bool OrderBatch::updateVolume(Order& order, const Volume newVolume)
{
    assert(order.volume >= newVolume);

    if (newVolume == 0)
    {
        return erase(order.id);
    }
    else
    {
        totalVolume_ -= order.volume - newVolume;
        order.volume = newVolume;
        return true;
    }
}

OrderBatch::Order& OrderBatch::topOrder()
{
    return orders_.front();
}

bool OrderBatch::empty() const
{
    return orders_.empty();
}

Volume OrderBatch::totalVolume() const
{
    return totalVolume_;
}
