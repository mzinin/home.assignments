#include <buy_order_finder.hpp>
#include <parse.hpp>
#include <types.hpp>

#include <catch2/catch.hpp>

TEST_CASE("BiggestBuyOrders :: No orders", "[biggest-buy-orders]")
{
    const auto record = recordFromString("10:00:00.000000;A;1;I;SELL;1;1");

    BuyOrderFinder finder;
    finder.add(record);
    const auto orders = finder.biggestBuyOrders(record.symbol);

    REQUIRE(orders.size() == 0);
}

TEST_CASE("BiggestBuyOrders :: No symbol orders", "[biggest-buy-orders]")
{
    const auto record = recordFromString("10:00:00.000000;A;1;I;BUY;1;1");

    BuyOrderFinder finder;
    finder.add(record);
    const auto orders = finder.biggestBuyOrders(record.symbol + 1);

    REQUIRE(orders.size() == 0);
}

TEST_CASE("BiggestBuyOrders :: One order", "[biggest-buy-orders]")
{
    const auto record = recordFromString("10:00:00.000000;A;1;I;BUY;1;1");

    BuyOrderFinder finder;
    finder.add(record);
    const auto orders = finder.biggestBuyOrders(record.symbol);

    REQUIRE(orders.size() == 1);
    CHECK(orders[0].ts == record.ts);
    CHECK(orders[0].symbol == record.symbol);
    CHECK(orders[0].orderID == record.orderID);
    CHECK(orders[0].volume == record.volume);
    CHECK(orders[0].price == record.price);
}

TEST_CASE("BiggestBuyOrders :: Two orders", "[biggest-buy-orders]")
{
    const auto record1 = recordFromString("10:00:00.000000;A;1;I;BUY;1;1");
    const auto record2 = recordFromString("10:00:01.000000;A;2;I;BUY;2;1");

    BuyOrderFinder finder;
    finder.add(record1);
    finder.add(record2);
    const auto orders = finder.biggestBuyOrders(record1.symbol);

    REQUIRE(orders.size() == 2);

    CHECK(orders[0].ts == record2.ts);
    CHECK(orders[0].symbol == record2.symbol);
    CHECK(orders[0].orderID == record2.orderID);
    CHECK(orders[0].volume == record2.volume);
    CHECK(orders[0].price == record2.price);

    CHECK(orders[1].ts == record1.ts);
    CHECK(orders[1].symbol == record1.symbol);
    CHECK(orders[1].orderID == record1.orderID);
    CHECK(orders[1].volume == record1.volume);
    CHECK(orders[1].price == record1.price);
}

TEST_CASE("BiggestBuyOrders :: Two orders, same volume", "[biggest-buy-orders]")
{
    const auto record1 = recordFromString("10:00:00.000000;A;1;I;BUY;1;2");
    const auto record2 = recordFromString("10:00:01.000000;A;2;I;BUY;1;1");

    BuyOrderFinder finder;
    finder.add(record1);
    finder.add(record2);
    const auto orders = finder.biggestBuyOrders(record1.symbol);

    REQUIRE(orders.size() == 2);

    CHECK(orders[0].ts == record1.ts);
    CHECK(orders[0].symbol == record1.symbol);
    CHECK(orders[0].orderID == record1.orderID);
    CHECK(orders[0].volume == record1.volume);
    CHECK(orders[0].price == record1.price);

    CHECK(orders[1].ts == record2.ts);
    CHECK(orders[1].symbol == record2.symbol);
    CHECK(orders[1].orderID == record2.orderID);
    CHECK(orders[1].volume == record2.volume);
    CHECK(orders[1].price == record2.price);
}

TEST_CASE("BiggestBuyOrders :: Canceled order", "[biggest-buy-orders]")
{
    const auto record1 = recordFromString("10:00:00.000000;A;1;I;BUY;2;1");
    const auto record2 = recordFromString("10:00:01.000000;A;2;I;BUY;1;1");
    const auto record3 = recordFromString("10:00:02.000000;A;1;C;BUY;2;1");

    BuyOrderFinder finder;
    finder.add(record1);
    finder.add(record2);
    finder.add(record3);
    const auto orders = finder.biggestBuyOrders(record1.symbol);

    REQUIRE(orders.size() == 1);

    CHECK(orders[0].ts == record2.ts);
    CHECK(orders[0].symbol == record2.symbol);
    CHECK(orders[0].orderID == record2.orderID);
    CHECK(orders[0].volume == record2.volume);
    CHECK(orders[0].price == record2.price);
}

TEST_CASE("BiggestBuyOrders :: Amended order", "[biggest-buy-orders]")
{
    const auto record1 = recordFromString("10:00:00.000000;A;1;I;BUY;5;1");
    const auto record2 = recordFromString("10:00:01.000000;A;2;I;BUY;3;1");
    const auto record3 = recordFromString("10:00:02.000000;A;1;A;BUY;1;1");

    BuyOrderFinder finder;
    finder.add(record1);
    finder.add(record2);
    finder.add(record3);
    const auto orders = finder.biggestBuyOrders(record1.symbol);

    REQUIRE(orders.size() == 2);

    CHECK(orders[0].ts == record2.ts);
    CHECK(orders[0].symbol == record2.symbol);
    CHECK(orders[0].orderID == record2.orderID);
    CHECK(orders[0].volume == record2.volume);
    CHECK(orders[0].price == record2.price);

    CHECK(orders[1].ts == record3.ts);
    CHECK(orders[1].symbol == record3.symbol);
    CHECK(orders[1].orderID == record3.orderID);
    CHECK(orders[1].volume == record3.volume);
    CHECK(orders[1].price == record3.price);
}

TEST_CASE("BiggestBuyOrders :: Many orders", "[biggest-buy-orders]")
{
    const auto record1 = recordFromString("10:00:00.000000;A;1;I;BUY;5;1");
    const auto record2 = recordFromString("10:00:01.000000;A;2;I;SELL;7;1");
    const auto record3 = recordFromString("10:00:02.000000;A;3;I;BUY;4;1");
    const auto record4 = recordFromString("10:00:03.000000;A;4;I;BUY;6;1");
    const auto record5 = recordFromString("10:00:04.000000;B;5;I;BUY;7;1");
    const auto record6 = recordFromString("10:00:05.000000;A;6;I;BUY;3;1");

    BuyOrderFinder finder;
    finder.add(record1);
    finder.add(record2);
    finder.add(record3);
    finder.add(record4);
    finder.add(record5);
    finder.add(record6);

    const auto ordersA = finder.biggestBuyOrders(record1.symbol);
    REQUIRE(ordersA.size() == 3);

    CHECK(ordersA[0].ts == record4.ts);
    CHECK(ordersA[0].symbol == record4.symbol);
    CHECK(ordersA[0].orderID == record4.orderID);
    CHECK(ordersA[0].volume == record4.volume);
    CHECK(ordersA[0].price == record4.price);

    CHECK(ordersA[1].ts == record1.ts);
    CHECK(ordersA[1].symbol == record1.symbol);
    CHECK(ordersA[1].orderID == record1.orderID);
    CHECK(ordersA[1].volume == record1.volume);
    CHECK(ordersA[1].price == record1.price);

    CHECK(ordersA[2].ts == record3.ts);
    CHECK(ordersA[2].symbol == record3.symbol);
    CHECK(ordersA[2].orderID == record3.orderID);
    CHECK(ordersA[2].volume == record3.volume);
    CHECK(ordersA[2].price == record3.price);

    const auto ordersB = finder.biggestBuyOrders(record5.symbol);
    REQUIRE(ordersB.size() == 1);

    CHECK(ordersB[0].ts == record5.ts);
    CHECK(ordersB[0].symbol == record5.symbol);
    CHECK(ordersB[0].orderID == record5.orderID);
    CHECK(ordersB[0].volume == record5.volume);
    CHECK(ordersB[0].price == record5.price);
}

