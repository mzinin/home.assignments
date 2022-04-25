#include "engine/matching_engine.hpp"

#include <algorithm>
#include <sstream>

namespace
{
    std::string symbolHeader(const Symbol& symbol)
    {
        std::ostringstream buffer;
        buffer << "===" << symbol << "===";
        return buffer.str();
    }
}

void MatchingEngine::process(const Event& event)
{
    switch (event.command)
    {
        case Command::AMEND:
            processAmend(event);
            break;

        case Command::INSERT:
            processInsert(event);
            break;

        case Command::PULL:
            processPull(event);
            break;
    }
}

std::vector<std::string> MatchingEngine::report() const
{
    std::vector<std::string> result;

    for (const auto& trade : trades_)
    {
        result.push_back(trade.toString());
    }

    std::vector<Symbol> symbols;
    for (const auto& [symbol, book] : books_)
    {
        symbols.push_back(symbol);
    }
    std::sort(symbols.begin(), symbols.end());

    for (const auto& symbol : symbols)
    {
        const auto& book = books_.at(symbol);
        const auto& items = book.getItems();
        if (!items.empty())
        {
            result.push_back(symbolHeader(symbol));
            for (const auto& item : items)
            {
                result.push_back(item.toString());
            }
        }
    }

    return result;
}

void MatchingEngine::processAmend(const Event& event)
{
    const auto orderIt = orders_.find(event.orderId);
    if (orderIt == orders_.end())
    {
        // no such order
        return;
    }

    // amend order within the corresponding book
    auto& book = findBook(orderIt->second.symbol);
    book.amend(event.orderId, orderIt->second.side, orderIt->second.price, event.price, event.volume);
}

void MatchingEngine::processInsert(const Event& event)
{
    const auto orderIt = orders_.find(event.orderId);
    if (orderIt != orders_.end())
    {
        // order already inserted
        return;
    }

    // save order info
    orders_[event.orderId] = OrderInfo{event.price, event.side, event.symbol};

    // insert order to the corresponding book
    auto& book = findBook(event.symbol);
    book.insert(event.orderId, event.side, event.price, event.volume);
}

void MatchingEngine::processPull(const Event& event)
{
    const auto orderIt = orders_.find(event.orderId);
    if (orderIt == orders_.end())
    {
        // no such order
        return;
    }

    // pull order from the corresponding book
    auto& book = findBook(orderIt->second.symbol);
    book.pull(event.orderId, orderIt->second.side, orderIt->second.price);
}

OrderBook& MatchingEngine::findBook(const Symbol symbol)
{
    auto bookIt = books_.try_emplace(
        symbol,
        symbol, // symbol of the new book
        [this](Trade&& trade){ addTrade(std::move(trade)); },     // trade handler
        [this](const OrderId orderId){ eraseOrderInfo(orderId); } // closed order handler
    ).first;
    return bookIt->second;
}

void MatchingEngine::addTrade(Trade&& trade)
{
    trades_.emplace_back(std::move(trade));
}

void MatchingEngine::eraseOrderInfo(const OrderId orderId)
{
    orders_.erase(orderId);
}
