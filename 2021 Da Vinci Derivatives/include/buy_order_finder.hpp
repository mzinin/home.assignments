#pragma once

#include <types.hpp>

#include <functional>
#include <set>
#include <unordered_map>
#include <vector>


class BuyOrderFinder final
{
public:
    struct BuyOrder
    {
        Timestamp ts;
        Symbol symbol;
        OrderID orderID;
        Volume volume;
        Price price;

        bool operator>(const BuyOrder& another) const;
    };

public:
    BuyOrderFinder() = default;

    void add(const Record& record);

    std::vector<BuyOrder> biggestBuyOrders(const Symbol& symbol) const;

private:
    void addOrder(const Record& record);
    void removeOrder(const Record& record);

private:
    struct BuyOrdersData
    {
        std::unordered_map<OrderID, std::multiset<BuyOrder>::iterator> activeOrders;
        std::multiset<BuyOrder, std::greater<BuyOrder>> sortedOrders;
    };

    std::unordered_map<Symbol, BuyOrdersData> buyOrders_;
};
