#pragma once

#include <types.hpp>

#include <unordered_map>
#include <unordered_set>


class OrderCounter final
{
public:
    OrderCounter() = default;

    void add(const Record& record);

    std::unordered_map<Symbol, uint32_t> orderCounts() const;

private:
    std::unordered_map<Symbol, std::unordered_set<OrderID>> activeOrders_;
    std::unordered_map<Symbol, uint32_t> numberOfOrders_;
};
