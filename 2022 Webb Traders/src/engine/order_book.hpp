#pragma once

#include "engine/order_batch.hpp"
#include "types/basic.hpp"
#include "types/book_item.hpp"
#include "types/trade.hpp"

#include <functional>
#include <list>
#include <map>
#include <vector>

// Covers all order with the same symbol.
class OrderBook final
{
public:
    using TradeHandler = std::function<void(Trade&&)>;
    using ClosedOrderHandler = std::function<void(const OrderId)>;

    // Constructor.
    // @param symbol[in] - Order's symbol of this book.
    // @param tradeHandler[in] - handler to call for every made trade.
    // @param closedOrderHandler[in] - handler to call for every closed order.
    OrderBook(const std::string_view symbol,
              TradeHandler tradeHandler,
              ClosedOrderHandler closedOrderHandler);

    // Amend an existing order.
    void amend(const OrderId orderId,
               const Side side,
               const Price oldPrice,
               const Price newPrice,
               const Volume newVolume);

    // Insert a new order.
    void insert(const OrderId orderId,
                const Side side,
                const Price price,
                const Volume volume);

    // Pull an existing order.
    void pull(const OrderId orderId,
              const Side side,
              const Price price);

    // Get all active items, sorted.
    std::vector<BookItem> getItems() const;

private:
    void updateBuyVolume(const OrderId orderId, const Price price, const Volume newVolume);
    void updateSellVolume(const OrderId orderId, const Price price, const Volume newVolume);

    void insertBuy(const OrderId orderId, const Price price, Volume volume);
    void insertSell(const OrderId orderId, const Price price, Volume volume);

    void commitBuyTrades(const Price price, OrderBatch& sellBatch, const OrderId buyOrderId, Volume& buyVolume);
    void commitSellTrades(const Price price, OrderBatch& buyBatch, const OrderId sellOrderId, Volume& sellVolume);

    bool pullBuy(const OrderId orderId, const Price price);
    bool pullSell(const OrderId orderId, const Price price);

private:
    const std::string_view symbol_;
    TradeHandler tradeHandler_;
    ClosedOrderHandler closedOrderHandler_;

    // All buying orders, grouped by price.
    std::map<Price, OrderBatch, std::greater<Price>> buys_;

    // All selling orders, grouped by price.
    std::map<Price, OrderBatch> sells_;
};
