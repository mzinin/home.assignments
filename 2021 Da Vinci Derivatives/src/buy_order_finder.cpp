#include <buy_order_finder.hpp>

namespace
{
    constexpr unsigned LIMIT = 3;
}

bool BuyOrderFinder::BuyOrder::operator>(const BuyOrder& another) const
{
    if (volume > another.volume) return true;
    else if (volume < another.volume) return false;
    else return price > another.price;
}

void BuyOrderFinder::add(const Record& record)
{
    if (record.side != Side::BUY)
    {
        return;
    }

    switch (record.operation)
    {
        case Operation::INSERT:
            addOrder(record);
            break;

        case Operation::CANCEL:
            removeOrder(record);
            break;

        case Operation::AMEND:
            removeOrder(record);
            addOrder(record);
            break;
    }
}

std::vector<BuyOrderFinder::BuyOrder> BuyOrderFinder::biggestBuyOrders(const Symbol& symbol) const
{
    std::vector<BuyOrderFinder::BuyOrder> result;

    const auto orderDataIt = buyOrders_.find(symbol);
    if (orderDataIt == buyOrders_.end())
    {
        return result;
    }

    const auto& sortedOrders = orderDataIt->second.sortedOrders;
    auto it = sortedOrders.begin();
    unsigned count = 0;

    while (count < LIMIT && it != sortedOrders.end())
    {
        result.push_back(*it);
        ++it;
        ++count;
    }

    return result;
}

void BuyOrderFinder::addOrder(const Record& record)
{
    auto& data = buyOrders_[record.symbol];

    auto it = data.sortedOrders.insert(BuyOrder{record.ts, record.symbol, record.orderID, record.volume, record.price});
    data.activeOrders[record.orderID] = it;
}

void BuyOrderFinder::removeOrder(const Record& record)
{
    const auto symbolDataIt = buyOrders_.find(record.symbol);
    if (symbolDataIt == buyOrders_.end())
    {
        return;
    }

    auto& data = symbolDataIt->second;
    const auto it = data.activeOrders.find(record.orderID);
    if (it == data.activeOrders.end())
    {
        return;
    }

    data.sortedOrders.erase(it->second);
    data.activeOrders.erase(it);

    if (data.activeOrders.empty())
    {
        buyOrders_.erase(symbolDataIt);
    }
}
