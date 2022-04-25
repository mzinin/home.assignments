#include <parse.hpp>

#include <catch2/catch.hpp>

TEST_CASE("TimestampFromString :: Micro second resolution", "[parse-timestamp]")
{
    const auto ts = timestampFromString("15:30:21.123456");
    CHECK(ts == 55821123456);
}

TEST_CASE("TimestampFromString :: Milli second resolution", "[parse-timestamp]")
{
    const auto ts = timestampFromString("15:30:21.123");
    CHECK(ts == 55821123000);
}

TEST_CASE("TimestampFromString :: Second resolution", "[parse-timestamp]")
{
    const auto ts = timestampFromString("15:30:00");
    CHECK(ts == 55800000000);
}

TEST_CASE("TimestampToString :: Micro second resolution", "[parse-timestamp]")
{
    const auto str = timestampToString(55821123456);
    CHECK(str == "15:30:21.123456");
}
