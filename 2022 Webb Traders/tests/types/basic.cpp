#include "../../src/types/basic.hpp"

#include <catch2/catch.hpp>

#include <sstream>

TEST_CASE("Types :: Basic :: Parse command")
{
    CHECK(parseCommand("AMEND") == Command::AMEND);
    CHECK(parseCommand("INSERT") == Command::INSERT);
    CHECK(parseCommand("PULL") == Command::PULL);
}

TEST_CASE("Types :: Basic :: Parse side")
{
    CHECK(parseSide("BUY") == Side::BUY);
    CHECK(parseSide("SELL") == Side::SELL);
}

TEST_CASE("Types :: Basic :: Parse order id")
{
    CHECK(parseOrderId("1") == 1);
    CHECK(parseOrderId("123") == 123);
}

TEST_CASE("Types :: Basic :: Parse volume")
{
    CHECK(parseVolume("0") == 0);
    CHECK(parseVolume("1") == 1);
    CHECK(parseVolume("1234") == 1234);
}

TEST_CASE("Types :: Basic :: Parse price")
{
    CHECK(parsePrice("0") == Price{0});
    CHECK(parsePrice("5") == Price{50000});
    CHECK(parsePrice("5.1") == Price{51000});
    CHECK(parsePrice("5.01") == Price{50100});
    CHECK(parsePrice("5.001") == Price{50010});
    CHECK(parsePrice("5.0001") == Price{50001});
}

TEST_CASE("Types :: Basic :: Compare price")
{
    CHECK(Price{150} == Price{150});
    CHECK(Price{150} < Price{250});
    CHECK(Price{150} > Price{125});
}

TEST_CASE("Types :: Basic :: Print price")
{
    std::ostringstream buffer;

    SECTION("0")
    {
        buffer << Price{0};
        CHECK(buffer.str() == "0");
    }

    SECTION("5")
    {
        buffer << Price{50000};
        CHECK(buffer.str() == "5");
    }

    SECTION("5.1")
    {
        buffer << Price{51000};
        CHECK(buffer.str() == "5.1");
    }

    SECTION("5.01")
    {
        buffer << Price{50100};
        CHECK(buffer.str() == "5.01");
    }

    SECTION("5.001")
    {
        buffer << Price{50010};
        CHECK(buffer.str() == "5.001");
    }

    SECTION("5.0001")
    {
        buffer << Price{50001};
        CHECK(buffer.str() == "5.0001");
    }

    SECTION("5.123")
    {
        buffer << Price{51230};
        CHECK(buffer.str() == "5.123");
    }
}
