#pragma once

#include <types.hpp>

#include <map>
#include <optional>
#include <unordered_map>
#include <vector>


class SellOrderFinder final
{
public:
    struct SellPosition
    {
        Price price;
        Volume volume;
    };

public:
    SellOrderFinder() = default;

    void add(const Record& record);

    std::optional<SellPosition> bestSellAtTime(const Symbol& symbol, const Timestamp& ts) const;

private:
    void addOrder(const Record& record);
    void removeOrder(const Record& record, bool updateHistory);

private:
    struct HistoryRecord
    {
        Timestamp ts;
        Price price;
        Volume volume;
    };

    struct SellOrdersData
    {
        std::unordered_map<OrderID, SellPosition> activeOrders;
        std::map<Price, Volume> activePrices;
        std::vector<HistoryRecord> priceHistory;
    };

    std::unordered_map<Symbol, SellOrdersData> sellOrders_;
};
