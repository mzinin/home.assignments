#include <sell_order_finder.hpp>

#include <algorithm>
#include <limits>

namespace
{
    constexpr Price IMPOSSIBLE_PRICE = std::numeric_limits<Price>::min();
}

void SellOrderFinder::add(const Record& record)
{
    if (record.side != Side::SELL)
    {
        return;
    }

    switch (record.operation)
    {
        case Operation::INSERT:
            addOrder(record);
            break;

        case Operation::CANCEL:
            removeOrder(record, true);
            break;

        case Operation::AMEND:
            removeOrder(record, false);
            addOrder(record);
            break;
    }
}

std::optional<SellOrderFinder::SellPosition> SellOrderFinder::bestSellAtTime(const Symbol& symbol, const Timestamp& ts) const
{
    std::optional<SellPosition> result;

    const auto orderDataIt = sellOrders_.find(symbol);
    if (orderDataIt == sellOrders_.end())
    {
        return result;
    }

    const auto& priceHistory = orderDataIt->second.priceHistory;

    const auto it = std::lower_bound(priceHistory.rbegin(),
                                     priceHistory.rend(),
                                     ts,
                                     [](const auto& a, const auto& b){ return a.ts > b; });
    if (it != priceHistory.rend() && it->price != IMPOSSIBLE_PRICE)
    {
        result = SellPosition{it->price, it->volume};
    }

    return result;
}

void SellOrderFinder::addOrder(const Record& record)
{
    auto& data = sellOrders_[record.symbol];

    data.activeOrders[record.orderID] = SellPosition{record.price, record.volume};
    data.activePrices[record.price] += record.volume;

    if (data.priceHistory.empty() ||
        data.priceHistory.back().price != data.activePrices.begin()->first ||
        data.priceHistory.back().volume != data.activePrices.begin()->second)
    {
        data.priceHistory.push_back(HistoryRecord{record.ts, data.activePrices.begin()->first, data.activePrices.begin()->second});
    }
}

void SellOrderFinder::removeOrder(const Record& record, bool updateHistory)
{
    // check if symbol is known
    const auto symbolDataIt = sellOrders_.find(record.symbol);
    if (symbolDataIt == sellOrders_.end())
    {
        return;
    }

    auto& data = symbolDataIt->second;

    // check is order is known
    const auto orderDataIt = data.activeOrders.find(record.orderID);
    if (orderDataIt == data.activeOrders.end())
    {
        return;
    }

    const auto& currentSell = orderDataIt->second;

    // descrease price's volume, remove if needed
    auto activePriceIt = data.activePrices.find(currentSell.price);
    activePriceIt->second -= currentSell.volume;
    if (activePriceIt->second == 0)
    {
        data.activePrices.erase(activePriceIt);
    }

    // update price history if needed
    if (updateHistory)
    {
        if (data.activePrices.empty())
        {
            data.priceHistory.push_back(HistoryRecord{record.ts, IMPOSSIBLE_PRICE, 0});
        }
        else if (data.priceHistory.back().price != data.activePrices.begin()->first ||
                 data.priceHistory.back().volume != data.activePrices.begin()->second)
        {
            data.priceHistory.push_back(HistoryRecord{record.ts, data.activePrices.begin()->first, data.activePrices.begin()->second});
        }
    }

    // remove order from the set of active orders
    data.activeOrders.erase(orderDataIt);
}