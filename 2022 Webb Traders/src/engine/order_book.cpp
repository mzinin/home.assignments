#include "engine/order_book.hpp"

OrderBook::OrderBook(const std::string_view symbol,
                     TradeHandler tradeHandler,
                     ClosedOrderHandler closedOrderHandler)
    : symbol_(symbol)
    , tradeHandler_(tradeHandler)
    , closedOrderHandler_(closedOrderHandler)
{
    // TODO check if handlers are not null?
}

void OrderBook::amend(const OrderId orderId,
                      const Side side,
                      const Price oldPrice,
                      const Price newPrice,
                      const Volume newVolume)
{
     // price is changed - pull an existing order and insert a new one
    if (oldPrice != newPrice)
    {
        // do not call pull here to avoid invoking closed order handler
        (side == Side::BUY) ? pullBuy(orderId, oldPrice) : pullSell(orderId, oldPrice);
        insert(orderId, side, newPrice, newVolume);
    }
    // if price is not changed, just update the volume
    else if (side == Side::BUY)
    {
        updateBuyVolume(orderId, oldPrice, newVolume);
    }
    else
    {
        updateSellVolume(orderId, oldPrice, newVolume);
    }

    if (newVolume == 0)
    {
        closedOrderHandler_(orderId);
    }
}

void OrderBook::insert(const OrderId orderId,
                       const Side side,
                       const Price price,
                       const Volume volume)
{
    if (side == Side::BUY)
    {
        insertBuy(orderId, price, volume);
    }
    else
    {
        insertSell(orderId, price, volume);
    }
}

void OrderBook::pull(const OrderId orderId,
                     const Side side,
                     const Price price)
{
    const bool orderPulled = (side == Side::BUY) ? pullBuy(orderId, price) : pullSell(orderId, price);
    if (orderPulled)
    {
        closedOrderHandler_(orderId);
    }
}

std::vector<BookItem> OrderBook::getItems() const
{
    std::vector<BookItem> result(std::max(buys_.size(), sells_.size()));

    size_t index = 0;
    for (const auto& [price, batch] : buys_)
    {
        result[index].setBuy(price, batch.totalVolume());
        ++index;
    }

    index = 0;
    for (const auto& [price, batch] : sells_)
    {
        result[index].setSell(price, batch.totalVolume());
        ++index;
    }

    return result;
}

void OrderBook::updateBuyVolume(const OrderId orderId, const Price price, const Volume newVolume)
{
    auto batchIt = buys_.find(price);
    if (batchIt != buys_.end())
    {
        batchIt->second.updateVolume(orderId, newVolume);
        if (batchIt->second.empty())
        {
            buys_.erase(batchIt);
        }
    }
}

void OrderBook::updateSellVolume(const OrderId orderId, const Price price, const Volume newVolume)
{
    auto batchIt = sells_.find(price);
    if (batchIt != sells_.end())
    {
        batchIt->second.updateVolume(orderId, newVolume);
        if (batchIt->second.empty())
        {
            sells_.erase(batchIt);
        }
    }
}

void OrderBook::insertBuy(const OrderId orderId, const Price price, Volume volume)
{
    auto sellsIt = sells_.begin();
    while (sellsIt != sells_.end() && sellsIt->first <= price)
    {
        commitBuyTrades(sellsIt->first, sellsIt->second, orderId, volume);
        if (sellsIt->second.empty())
        {
            sellsIt = sells_.erase(sellsIt);
        }
        else
        {
            break;
        }
    }

    if (volume != 0)
    {
        buys_[price].add(orderId, volume);
    }
    else
    {
        closedOrderHandler_(orderId);
    }
}

void OrderBook::insertSell(const OrderId orderId, const Price price, Volume volume)
{
    auto buyIt = buys_.begin();
    while (buyIt != buys_.end() && buyIt->first >= price)
    {
        commitSellTrades(buyIt->first, buyIt->second, orderId, volume);
        if (buyIt->second.empty())
        {
            buyIt = buys_.erase(buyIt);
        }
        else
        {
            break;
        }
    }

    if (volume != 0)
    {
        sells_[price].add(orderId, volume);
    }
    else
    {
        closedOrderHandler_(orderId);
    }
}

void OrderBook::commitBuyTrades(const Price price, OrderBatch& sellBatch, const OrderId buyOrderId, Volume& buyVolume)
{
    while (!sellBatch.empty() && buyVolume != 0)
    {
        // get chonologically first order
        auto& sellOrder = sellBatch.topOrder();

        // commit a trade
        const auto tradeVolume = std::min(sellOrder.volume, buyVolume);
        tradeHandler_(Trade{price, tradeVolume, buyOrderId, sellOrder.id, symbol_});

        // close selling order if needed
        if (sellOrder.volume == tradeVolume)
        {
            closedOrderHandler_(sellOrder.id);
        }

        buyVolume -= tradeVolume;
        sellBatch.updateVolume(sellOrder, sellOrder.volume - tradeVolume);
    }
}

void OrderBook::commitSellTrades(const Price price, OrderBatch& buyBatch, const OrderId sellOrderId, Volume& sellVolume)
{
    while (!buyBatch.empty() && sellVolume != 0)
    {
        // get chonologically first order
        auto& buyOrder = buyBatch.topOrder();

        // commit a trade
        const auto tradeVolume = std::min(buyOrder.volume, sellVolume);
        tradeHandler_(Trade{price, tradeVolume, sellOrderId, buyOrder.id, symbol_});

        // close buying order if needed
        if (buyOrder.volume == tradeVolume)
        {
            closedOrderHandler_(buyOrder.id);
        }

        sellVolume -= tradeVolume;
        buyBatch.updateVolume(buyOrder, buyOrder.volume - tradeVolume);
    }
}

bool OrderBook::pullBuy(const OrderId orderId, const Price price)
{
    bool result = false;
    auto batchIt = buys_.find(price);
    if (batchIt != buys_.end())
    {
        result = batchIt->second.erase(orderId);
        if (batchIt->second.empty())
        {
            buys_.erase(batchIt);
        }
    }
    return result;
}

bool OrderBook::pullSell(const OrderId orderId, const Price price)
{
    bool result = false;
    auto batchIt = sells_.find(price);
    if (batchIt != sells_.end())
    {
        result = batchIt->second.erase(orderId);
        if (batchIt->second.empty())
        {
            sells_.erase(batchIt);
        }
    }
    return result;
}
