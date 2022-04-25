#pragma once

#include <engine/order_book.hpp>
#include "types/event.hpp"
#include "types/trade.hpp"

#include <deque>
#include <string>
#include <unordered_map>
#include <vector>

// Top-level matching engine, implements all logic.
class MatchingEngine final
{
public:
    MatchingEngine() = default;

    // Process one event.
    void process(const Event& event);

    // Generate a final report.
    std::vector<std::string> report() const;

private:
    // Process one amending order event.
    void processAmend(const Event& event);

    // Process one inserting order event.
    void processInsert(const Event& event);

    // Process one pulling order event.
    void processPull(const Event& event);

    // Find existing or create a new order book for the provided symbol.
    OrderBook& findBook(const Symbol symbol);

    // Add a new actual trade.
    void addTrade(Trade&& trade);

    // Erase info about an existing order.
    void eraseOrderInfo(const OrderId orderId);

private:
    struct OrderInfo final
    {
        Price price;
        Side side;
        Symbol symbol;
    };

    std::unordered_map<OrderId, OrderInfo> orders_;
    std::unordered_map<Symbol, OrderBook> books_;
    std::deque<Trade> trades_;
};
