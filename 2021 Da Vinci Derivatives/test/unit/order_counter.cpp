#include <order_counter.hpp>
#include <parse.hpp>
#include <types.hpp>

#include <catch2/catch.hpp>

TEST_CASE("OrderCounts :: One inserted order", "[order-counts]")
{
    const auto record = recordFromString("10:00:00.000000;A;1;I;BUY;1;1");

    OrderCounter counter;
    counter.add(record);
    const auto orders = counter.orderCounts();

    REQUIRE(orders.size() == 1);
    CHECK(orders.at(record.symbol) == 1);
}

TEST_CASE("OrderCounts :: One cancelled order", "[order-counts]")
{
    const auto record = recordFromString("10:00:00.000000;A;1;C;BUY;1;1");

    OrderCounter counter;
    counter.add(record);
    const auto orders = counter.orderCounts();

    CHECK(orders.size() == 0);
}

TEST_CASE("OrderCounts :: Inserted and canceleld order", "[order-counts]")
{
    const auto record1 = recordFromString("10:00:00.000000;A;1;I;BUY;1;1");
    const auto record2 = recordFromString("10:00:01.000000;A;1;C;BUY;1;1");

    OrderCounter counter;
    counter.add(record1);
    counter.add(record2);
    const auto orders = counter.orderCounts();

    CHECK(orders.size() == 0);
}

TEST_CASE("OrderCounts :: Two orders, two symbols", "[order-counts]")
{
    const auto record1 = recordFromString("10:00:00.000000;A;1;I;BUY;1;1");
    const auto record2 = recordFromString("10:00:01.000000;B;2;I;SELL;1;1");

    OrderCounter counter;
    counter.add(record1);
    counter.add(record2);
    const auto orders = counter.orderCounts();

    REQUIRE(orders.size() == 2);
    CHECK(orders.at(record1.symbol) == 1);
    CHECK(orders.at(record2.symbol) == 1);
}

TEST_CASE("OrderCounts :: Two orders, one symbol", "[order-counts]")
{
    const auto record1 = recordFromString("10:00:00.000000;A;1;I;BUY;1;1");
    const auto record2 = recordFromString("10:00:01.000000;A;2;I;BUY;2;2");

    OrderCounter counter;
    counter.add(record1);
    counter.add(record2);
    const auto orders = counter.orderCounts();

    REQUIRE(orders.size() == 1);
    CHECK(orders.at(record1.symbol) == 2);
}
