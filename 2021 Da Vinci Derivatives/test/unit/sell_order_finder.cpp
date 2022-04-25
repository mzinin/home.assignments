#include <parse.hpp>
#include <sell_order_finder.hpp>
#include <types.hpp>

#include <catch2/catch.hpp>

TEST_CASE("BestSellAtTime :: No orders", "[best-sell-at-time]")
{
    const auto record = recordFromString("10:00:00.000000;A;1;I;BUY;1;1");

    SellOrderFinder finder;
    finder.add(record);
    const auto bestSell = finder.bestSellAtTime(record.symbol, record.ts + 1);

    REQUIRE_FALSE(bestSell);
}

TEST_CASE("BestSellAtTime :: No symbol orders", "[best-sell-at-time]")
{
    const auto record = recordFromString("10:00:00.000000;A;1;I;SELL;1;1");

    SellOrderFinder finder;
    finder.add(record);
    const auto bestSell = finder.bestSellAtTime(record.symbol + 1, record.ts + 1);

    REQUIRE_FALSE(bestSell);
}

TEST_CASE("BestSellAtTime :: No orders yet", "[best-sell-at-time]")
{
    const auto record = recordFromString("10:00:00.000000;A;1;I;SELL;1;1");

    SellOrderFinder finder;
    finder.add(record);
    const auto bestSell = finder.bestSellAtTime(record.symbol, record.ts - 1);

    REQUIRE_FALSE(bestSell);
}

TEST_CASE("BestSellAtTime :: One order right now", "[best-sell-at-time]")
{
    const auto record = recordFromString("10:00:00.000000;A;1;I;SELL;1;1");

    SellOrderFinder finder;
    finder.add(record);
    const auto bestSell = finder.bestSellAtTime(record.symbol, record.ts);

    REQUIRE(bestSell);
    CHECK(bestSell->price == record.price);
    CHECK(bestSell->volume == record.volume);
}

TEST_CASE("BestSellAtTime :: One order in the past", "[best-sell-at-time]")
{
    const auto record = recordFromString("10:00:00.000000;A;1;I;SELL;1;1");

    SellOrderFinder finder;
    finder.add(record);
    const auto bestSell = finder.bestSellAtTime(record.symbol, record.ts + 100);

    REQUIRE(bestSell);
    CHECK(bestSell->price == record.price);
    CHECK(bestSell->volume == record.volume);
}

TEST_CASE("BestSellAtTime :: Amended order", "[best-sell-at-time]")
{
    const auto record1 = recordFromString("10:00:00.000000;A;1;I;SELL;1;2");
    const auto record2 = recordFromString("10:00:01.000000;A;1;A;SELL;3;4");

    SellOrderFinder finder;
    finder.add(record1);
    finder.add(record2);

    const auto bestSell1 = finder.bestSellAtTime(record1.symbol, record1.ts);
    REQUIRE(bestSell1);
    CHECK(bestSell1->price == record1.price);
    CHECK(bestSell1->volume == record1.volume);

    const auto bestSell2 = finder.bestSellAtTime(record2.symbol, record2.ts);
    REQUIRE(bestSell2);
    CHECK(bestSell2->price == record2.price);
    CHECK(bestSell2->volume == record2.volume);
}

TEST_CASE("BestSellAtTime :: Cancelled order", "[best-sell-at-time]")
{
    const auto record1 = recordFromString("10:00:00.000000;A;1;I;SELL;1;2");
    const auto record2 = recordFromString("10:00:01.000000;A;1;C;SELL;1;2");

    SellOrderFinder finder;
    finder.add(record1);
    finder.add(record2);

    const auto bestSell1 = finder.bestSellAtTime(record1.symbol, record1.ts);
    REQUIRE(bestSell1);
    CHECK(bestSell1->price == record1.price);
    CHECK(bestSell1->volume == record1.volume);

    const auto bestSell2 = finder.bestSellAtTime(record2.symbol, record2.ts);
    REQUIRE_FALSE(bestSell2);
}

TEST_CASE("BestSellAtTime :: Two orders", "[best-sell-at-time]")
{
    const auto record1 = recordFromString("10:00:00.000000;A;1;I;SELL;1;1");
    const auto record2 = recordFromString("10:00:01.000000;A;2;I;SELL;1;2");

    SellOrderFinder finder;
    finder.add(record1);
    finder.add(record2);

    const auto bestSell0 = finder.bestSellAtTime(record1.symbol, record1.ts - 1);
    REQUIRE_FALSE(bestSell0);

    const auto bestSell1 = finder.bestSellAtTime(record1.symbol, record1.ts);
    REQUIRE(bestSell1);
    CHECK(bestSell1->price == record1.price);
    CHECK(bestSell1->volume == record1.volume);

    const auto bestSell2 = finder.bestSellAtTime(record2.symbol, record2.ts);
    REQUIRE(bestSell2);
    CHECK(bestSell2->price == record1.price);
    CHECK(bestSell2->volume == record1.volume);
}

TEST_CASE("BestSellAtTime :: Two orders, same price", "[best-sell-at-time]")
{
    const auto record1 = recordFromString("10:00:00.000000;A;1;I;SELL;1;1");
    const auto record2 = recordFromString("10:00:01.000000;A;2;I;SELL;2;1");

    SellOrderFinder finder;
    finder.add(record1);
    finder.add(record2);

    const auto bestSell1 = finder.bestSellAtTime(record1.symbol, record1.ts);
    REQUIRE(bestSell1);
    CHECK(bestSell1->price == record1.price);
    CHECK(bestSell1->volume == record1.volume);

    const auto bestSell2 = finder.bestSellAtTime(record2.symbol, record2.ts);
    REQUIRE(bestSell2);
    CHECK(bestSell2->price == record1.price);
    CHECK(bestSell2->volume == record1.volume + record2.volume);
}

TEST_CASE("BestSellAtTime :: Three orders", "[best-sell-at-time]")
{
    const auto record1 = recordFromString("10:00:00.000000;A;1;I;SELL;1;2");
    const auto record2 = recordFromString("10:00:01.000000;A;2;I;SELL;2;2");
    const auto record3 = recordFromString("10:00:02.000000;A;3;I;SELL;1;1");

    SellOrderFinder finder;
    finder.add(record1);
    finder.add(record2);
    finder.add(record3);

    const auto bestSell1 = finder.bestSellAtTime(record1.symbol, record1.ts);
    REQUIRE(bestSell1);
    CHECK(bestSell1->price == record1.price);
    CHECK(bestSell1->volume == record1.volume);

    const auto bestSell2 = finder.bestSellAtTime(record2.symbol, record2.ts);
    REQUIRE(bestSell2);
    CHECK(bestSell2->price == record1.price);
    CHECK(bestSell2->volume == record1.volume + record2.volume);

    const auto bestSell3 = finder.bestSellAtTime(record3.symbol, record3.ts);
    REQUIRE(bestSell3);
    CHECK(bestSell3->price == record3.price);
    CHECK(bestSell3->volume == record3.volume);
}

TEST_CASE("BestSellAtTime :: Many orders", "[best-sell-at-time]")
{
    const auto record1 = recordFromString("10:00:00.000000;A;1;I;SELL;5;1");
    const auto record2 = recordFromString("10:00:01.000000;B;2;I;SELL;3;10");
    const auto record3 = recordFromString("10:00:02.000000;C;3;I;SELL;1;100");
    const auto record4 = recordFromString("10:00:00.000000;A;4;I;SELL;1;2");
    const auto record5 = recordFromString("10:00:01.000000;B;5;I;SELL;5;9");
    const auto record6 = recordFromString("10:00:02.000000;C;6;I;SELL;2;80");

    SellOrderFinder finder;
    finder.add(record1);
    finder.add(record2);
    finder.add(record3);
    finder.add(record4);
    finder.add(record5);
    finder.add(record6);

    const auto bestSellA = finder.bestSellAtTime(record1.symbol, record6.ts);
    REQUIRE(bestSellA);
    CHECK(bestSellA->price == record1.price);
    CHECK(bestSellA->volume == record1.volume);

    const auto bestSellB = finder.bestSellAtTime(record2.symbol, record6.ts);
    REQUIRE(bestSellB);
    CHECK(bestSellB->price == record5.price);
    CHECK(bestSellB->volume == record5.volume);

    const auto bestSellC = finder.bestSellAtTime(record3.symbol, record6.ts);
    REQUIRE(bestSellC);
    CHECK(bestSellC->price == record6.price);
    CHECK(bestSellC->volume == record6.volume);
}
