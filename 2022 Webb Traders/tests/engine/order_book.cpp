#include "../../src/engine/order_book.hpp"

#include <catch2/catch.hpp>

#include <vector>

TEST_CASE("Engine :: Book :: Empty")
{
    std::vector<Trade> trades;
    auto tradeHandler = [&trades](Trade&& trade){ trades.push_back(std::move(trade)); };

    std::vector<OrderId> closedOrders;
    auto orderHandler = [&closedOrders](const OrderId id){ closedOrders.push_back(id); };

    OrderBook book{"A", tradeHandler, orderHandler};

    CHECK(book.getItems().empty());
    CHECK(trades.empty());
    CHECK(closedOrders.empty());
}

TEST_CASE("Engine :: Book :: Insert one order")
{
    std::vector<Trade> trades;
    auto tradeHandler = [&trades](Trade&& trade){ trades.push_back(std::move(trade)); };

    std::vector<OrderId> closedOrders;
    auto orderHandler = [&closedOrders](const OrderId id){ closedOrders.push_back(id); };

    OrderBook book{"A", tradeHandler, orderHandler};
    book.insert(1, Side::BUY, Price{1}, 100);

    CHECK(trades.empty());
    CHECK(closedOrders.empty());

    const auto items = book.getItems();
    CHECK(items.size() == 1);
    CHECK(*items[0].buyPrice == Price{1});
    CHECK(*items[0].buyVolume == 100);
    CHECK(!items[0].sellPrice);
    CHECK(!items[0].sellVolume);
}

TEST_CASE("Engine :: Book :: Insert two orders")
{
    std::vector<Trade> trades;
    auto tradeHandler = [&trades](Trade&& trade){ trades.push_back(std::move(trade)); };

    std::vector<OrderId> closedOrders;
    auto orderHandler = [&closedOrders](const OrderId id){ closedOrders.push_back(id); };

    OrderBook book{"A", tradeHandler, orderHandler};
    book.insert(1, Side::BUY, Price{1}, 100);
    book.insert(2, Side::SELL, Price{2}, 200);

    CHECK(trades.empty());
    CHECK(closedOrders.empty());

    const auto items = book.getItems();
    CHECK(items.size() == 1);
    CHECK(*items[0].buyPrice == Price{1});
    CHECK(*items[0].buyVolume == 100);
    CHECK(*items[0].sellPrice == Price{2});
    CHECK(*items[0].sellVolume == 200);
}

TEST_CASE("Engine :: Book :: Insert two buy orders")
{
    std::vector<Trade> trades;
    auto tradeHandler = [&trades](Trade&& trade){ trades.push_back(std::move(trade)); };

    std::vector<OrderId> closedOrders;
    auto orderHandler = [&closedOrders](const OrderId id){ closedOrders.push_back(id); };

    OrderBook book{"A", tradeHandler, orderHandler};
    book.insert(1, Side::BUY, Price{1}, 100);
    book.insert(2, Side::BUY, Price{2}, 200);

    CHECK(trades.empty());
    CHECK(closedOrders.empty());

    const auto items = book.getItems();
    CHECK(items.size() == 2);
    CHECK(*items[0].buyPrice == Price{2});
    CHECK(*items[0].buyVolume == 200);
    CHECK(!items[0].sellPrice);
    CHECK(!items[0].sellVolume);
    CHECK(*items[1].buyPrice == Price{1});
    CHECK(*items[1].buyVolume == 100);
    CHECK(!items[1].sellPrice);
    CHECK(!items[1].sellVolume);
}

TEST_CASE("Engine :: Book :: Insert two sell orders")
{
    std::vector<Trade> trades;
    auto tradeHandler = [&trades](Trade&& trade){ trades.push_back(std::move(trade)); };

    std::vector<OrderId> closedOrders;
    auto orderHandler = [&closedOrders](const OrderId id){ closedOrders.push_back(id); };

    OrderBook book{"A", tradeHandler, orderHandler};
    book.insert(1, Side::SELL, Price{1}, 100);
    book.insert(2, Side::SELL, Price{2}, 200);

    CHECK(trades.empty());
    CHECK(closedOrders.empty());

    const auto items = book.getItems();
    CHECK(items.size() == 2);
    CHECK(!items[0].buyPrice);
    CHECK(!items[0].buyVolume);
    CHECK(*items[0].sellPrice == Price{1});
    CHECK(*items[0].sellVolume == 100);
    CHECK(!items[1].buyPrice);
    CHECK(!items[1].buyVolume);
    CHECK(*items[1].sellPrice == Price{2});
    CHECK(*items[1].sellVolume == 200);
}

TEST_CASE("Engine :: Book :: Commit full buy trade")
{
    std::vector<Trade> trades;
    auto tradeHandler = [&trades](Trade&& trade){ trades.push_back(std::move(trade)); };

    std::vector<OrderId> closedOrders;
    auto orderHandler = [&closedOrders](const OrderId id){ closedOrders.push_back(id); };

    OrderBook book{"A", tradeHandler, orderHandler};
    book.insert(1, Side::SELL, Price{10}, 100);
    book.insert(2, Side::BUY, Price{12}, 50);

    // check trades
    CHECK(trades.size() == 1);
    CHECK(trades[0].price == Price{10});
    CHECK(trades[0].volume == 50);
    CHECK(trades[0].aggressiveOrderId == 2);
    CHECK(trades[0].passiveOrderId == 1);
    CHECK(trades[0].symbol == "A");

    // check closed orders
    CHECK(closedOrders.size() == 1);
    CHECK(closedOrders[0] == 2);

    const auto items = book.getItems();
    CHECK(items.size() == 1);
    CHECK(!items[0].buyPrice);
    CHECK(!items[0].buyVolume);
    CHECK(*items[0].sellPrice == Price{10});
    CHECK(*items[0].sellVolume == 50);
}

TEST_CASE("Engine :: Book :: Commit partial buy trade")
{
    std::vector<Trade> trades;
    auto tradeHandler = [&trades](Trade&& trade){ trades.push_back(std::move(trade)); };

    std::vector<OrderId> closedOrders;
    auto orderHandler = [&closedOrders](const OrderId id){ closedOrders.push_back(id); };

    OrderBook book{"A", tradeHandler, orderHandler};
    book.insert(1, Side::SELL, Price{10}, 100);
    book.insert(2, Side::BUY, Price{12}, 200);

    // check trades
    CHECK(trades.size() == 1);
    CHECK(trades[0].price == Price{10});
    CHECK(trades[0].volume == 100);
    CHECK(trades[0].aggressiveOrderId == 2);
    CHECK(trades[0].passiveOrderId == 1);
    CHECK(trades[0].symbol == "A");

    // check closed orders
    CHECK(closedOrders.size() == 1);
    CHECK(closedOrders[0] == 1);

    const auto items = book.getItems();
    CHECK(items.size() == 1);
    CHECK(*items[0].buyPrice == Price{12});
    CHECK(*items[0].buyVolume == 100);
    CHECK(!items[0].sellPrice);
    CHECK(!items[0].sellVolume);
}

TEST_CASE("Engine :: Book :: Commit full sell trade")
{
    std::vector<Trade> trades;
    auto tradeHandler = [&trades](Trade&& trade){ trades.push_back(std::move(trade)); };

    std::vector<OrderId> closedOrders;
    auto orderHandler = [&closedOrders](const OrderId id){ closedOrders.push_back(id); };

    OrderBook book{"A", tradeHandler, orderHandler};
    book.insert(1, Side::BUY, Price{12}, 150);
    book.insert(2, Side::SELL, Price{12}, 50);

    // check trades
    CHECK(trades.size() == 1);
    CHECK(trades[0].price == Price{12});
    CHECK(trades[0].volume == 50);
    CHECK(trades[0].aggressiveOrderId == 2);
    CHECK(trades[0].passiveOrderId == 1);
    CHECK(trades[0].symbol == "A");

    // check closed orders
    CHECK(closedOrders.size() == 1);
    CHECK(closedOrders[0] == 2);

    const auto items = book.getItems();
    CHECK(items.size() == 1);
    CHECK(*items[0].buyPrice == Price{12});
    CHECK(*items[0].buyVolume == 100);
    CHECK(!items[0].sellPrice);
    CHECK(!items[0].sellVolume);
}

TEST_CASE("Engine :: Book :: Commit partial sell trade")
{
    std::vector<Trade> trades;
    auto tradeHandler = [&trades](Trade&& trade){ trades.push_back(std::move(trade)); };

    std::vector<OrderId> closedOrders;
    auto orderHandler = [&closedOrders](const OrderId id){ closedOrders.push_back(id); };

    OrderBook book{"A", tradeHandler, orderHandler};
    book.insert(1, Side::BUY, Price{15}, 200);
    book.insert(2, Side::SELL, Price{12}, 500);

    // check trades
    CHECK(trades.size() == 1);
    CHECK(trades[0].price == Price{15});
    CHECK(trades[0].volume == 200);
    CHECK(trades[0].aggressiveOrderId == 2);
    CHECK(trades[0].passiveOrderId == 1);
    CHECK(trades[0].symbol == "A");

    // check closed orders
    CHECK(closedOrders.size() == 1);
    CHECK(closedOrders[0] == 1);

    const auto items = book.getItems();
    CHECK(items.size() == 1);
    CHECK(!items[0].buyPrice);
    CHECK(!items[0].buyVolume);
    CHECK(*items[0].sellPrice == Price{12});
    CHECK(*items[0].sellVolume == 300);
}

TEST_CASE("Engine :: Book :: Pull existing order")
{
    std::vector<Trade> trades;
    auto tradeHandler = [&trades](Trade&& trade){ trades.push_back(std::move(trade)); };

    std::vector<OrderId> closedOrders;
    auto orderHandler = [&closedOrders](const OrderId id){ closedOrders.push_back(id); };

    OrderBook book{"A", tradeHandler, orderHandler};
    book.insert(1, Side::BUY, Price{10}, 100);
    book.insert(2, Side::SELL, Price{12}, 200);
    book.insert(3, Side::BUY, Price{11}, 300);
    book.pull(1, Side::BUY, Price{10});

    CHECK(trades.empty());
    CHECK(closedOrders.size() == 1);
    CHECK(closedOrders[0] == 1);

    const auto items = book.getItems();
    CHECK(items.size() == 1);
    CHECK(*items[0].buyPrice == Price{11});
    CHECK(*items[0].buyVolume == 300);
    CHECK(*items[0].sellPrice == Price{12});
    CHECK(*items[0].sellVolume == 200);
}

TEST_CASE("Engine :: Book :: Pull order with wrong id")
{
    std::vector<Trade> trades;
    auto tradeHandler = [&trades](Trade&& trade){ trades.push_back(std::move(trade)); };

    std::vector<OrderId> closedOrders;
    auto orderHandler = [&closedOrders](const OrderId id){ closedOrders.push_back(id); };

    OrderBook book{"A", tradeHandler, orderHandler};
    book.insert(1, Side::BUY, Price{10}, 100);
    book.insert(2, Side::SELL, Price{12}, 200);
    book.insert(3, Side::BUY, Price{11}, 300);
    book.pull(4, Side::BUY, Price{10});

    CHECK(trades.empty());
    CHECK(closedOrders.empty());

    const auto items = book.getItems();
    CHECK(items.size() == 2);
    CHECK(*items[0].buyPrice == Price{11});
    CHECK(*items[0].buyVolume == 300);
    CHECK(*items[0].sellPrice == Price{12});
    CHECK(*items[0].sellVolume == 200);
    CHECK(*items[1].buyPrice == Price{10});
    CHECK(*items[1].buyVolume == 100);
    CHECK(!items[1].sellPrice);
    CHECK(!items[1].sellVolume);
}

TEST_CASE("Engine :: Book :: Pull order with wrong side")
{
    std::vector<Trade> trades;
    auto tradeHandler = [&trades](Trade&& trade){ trades.push_back(std::move(trade)); };

    std::vector<OrderId> closedOrders;
    auto orderHandler = [&closedOrders](const OrderId id){ closedOrders.push_back(id); };

    OrderBook book{"A", tradeHandler, orderHandler};
    book.insert(1, Side::BUY, Price{10}, 100);
    book.insert(2, Side::SELL, Price{12}, 200);
    book.insert(3, Side::BUY, Price{11}, 300);
    book.pull(1, Side::SELL, Price{10});

    CHECK(trades.empty());
    CHECK(closedOrders.empty());
    CHECK(book.getItems().size() == 2);
}

TEST_CASE("Engine :: Book :: Pull order with wrong price")
{
    std::vector<Trade> trades;
    auto tradeHandler = [&trades](Trade&& trade){ trades.push_back(std::move(trade)); };

    std::vector<OrderId> closedOrders;
    auto orderHandler = [&closedOrders](const OrderId id){ closedOrders.push_back(id); };

    OrderBook book{"A", tradeHandler, orderHandler};
    book.insert(1, Side::BUY, Price{10}, 100);
    book.insert(2, Side::SELL, Price{12}, 200);
    book.insert(3, Side::BUY, Price{11}, 300);
    book.pull(1, Side::BUY, Price{11});

    CHECK(trades.empty());
    CHECK(closedOrders.empty());
    CHECK(book.getItems().size() == 2);
}

TEST_CASE("Engine :: Book :: Increase price")
{
    std::vector<Trade> trades;
    auto tradeHandler = [&trades](Trade&& trade){ trades.push_back(std::move(trade)); };

    std::vector<OrderId> closedOrders;
    auto orderHandler = [&closedOrders](const OrderId id){ closedOrders.push_back(id); };

    OrderBook book{"A", tradeHandler, orderHandler};
    book.insert(1, Side::BUY, Price{10}, 100);
    book.insert(2, Side::BUY, Price{11}, 300);
    book.amend(1, Side::BUY, Price{10}, Price{12}, 100);

    CHECK(trades.empty());
    CHECK(closedOrders.empty());

    const auto items = book.getItems();
    CHECK(items.size() == 2);
    CHECK(*items[0].buyPrice == Price{12});
    CHECK(*items[0].buyVolume == 100);
    CHECK(*items[1].buyPrice == Price{11});
    CHECK(*items[1].buyVolume == 300);
}

TEST_CASE("Engine :: Book :: Decrease price")
{
    std::vector<Trade> trades;
    auto tradeHandler = [&trades](Trade&& trade){ trades.push_back(std::move(trade)); };

    std::vector<OrderId> closedOrders;
    auto orderHandler = [&closedOrders](const OrderId id){ closedOrders.push_back(id); };

    OrderBook book{"A", tradeHandler, orderHandler};
    book.insert(1, Side::BUY, Price{10}, 100);
    book.insert(2, Side::BUY, Price{11}, 300);
    book.amend(2, Side::BUY, Price{11}, Price{8}, 300);

    CHECK(trades.empty());
    CHECK(closedOrders.empty());

    const auto items = book.getItems();
    CHECK(items.size() == 2);
    CHECK(*items[0].buyPrice == Price{10});
    CHECK(*items[0].buyVolume == 100);
    CHECK(*items[1].buyPrice == Price{8});
    CHECK(*items[1].buyVolume == 300);
}

TEST_CASE("Engine :: Book :: Increase volume")
{
    std::vector<Trade> trades;
    auto tradeHandler = [&trades](Trade&& trade){ trades.push_back(std::move(trade)); };

    std::vector<OrderId> closedOrders;
    auto orderHandler = [&closedOrders](const OrderId id){ closedOrders.push_back(id); };

    OrderBook book{"A", tradeHandler, orderHandler};
    book.insert(1, Side::BUY, Price{10}, 100);
    book.insert(2, Side::BUY, Price{10}, 300);
    book.amend(1, Side::BUY, Price{10}, Price{10}, 200);
    book.insert(3, Side::SELL, Price{10}, 100);

    // check trades
    CHECK(trades.size() == 1);
    CHECK(trades[0].price == Price{10});
    CHECK(trades[0].volume == 100);
    CHECK(trades[0].aggressiveOrderId == 3);
    CHECK(trades[0].passiveOrderId == 2);
    CHECK(trades[0].symbol == "A");

    // check closed orders
    CHECK(closedOrders.size() == 1);
    CHECK(closedOrders[0] == 3);

    // check book's items
    const auto items = book.getItems();
    CHECK(items.size() == 1);
    CHECK(*items[0].buyPrice == Price{10});
    CHECK(*items[0].buyVolume == 400);
}

TEST_CASE("Engine :: Book :: Descrease volume")
{
    std::vector<Trade> trades;
    auto tradeHandler = [&trades](Trade&& trade){ trades.push_back(std::move(trade)); };

    std::vector<OrderId> closedOrders;
    auto orderHandler = [&closedOrders](const OrderId id){ closedOrders.push_back(id); };

    OrderBook book{"A", tradeHandler, orderHandler};
    book.insert(1, Side::BUY, Price{10}, 100);
    book.insert(2, Side::BUY, Price{10}, 300);
    book.amend(1, Side::BUY, Price{10}, Price{10}, 80);
    book.insert(3, Side::SELL, Price{10}, 50);

    // check trades
    CHECK(trades.size() == 1);
    CHECK(trades[0].price == Price{10});
    CHECK(trades[0].volume == 50);
    CHECK(trades[0].aggressiveOrderId == 3);
    CHECK(trades[0].passiveOrderId == 1);
    CHECK(trades[0].symbol == "A");

    // check closed orders
    CHECK(closedOrders.size() == 1);
    CHECK(closedOrders[0] == 3);

    // check book's items
    const auto items = book.getItems();
    CHECK(items.size() == 1);
    CHECK(*items[0].buyPrice == Price{10});
    CHECK(*items[0].buyVolume == 330);
}
