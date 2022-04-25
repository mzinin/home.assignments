#include <parse.hpp>

#include <catch2/catch.hpp>

TEST_CASE("SymbolFromString :: Simple conversion", "[parse-symbol]")
{
    const auto symbol = symbolFromString("A");
    CHECK(symbol == 65);
}

TEST_CASE("SymbolToString :: Simple conversion", "[parse-symbol]")
{
    const auto str = symbolToString(65);
    CHECK(str == "A");
}

TEST_CASE("SymbolFromString :: Real conversion", "[parse-symbol]")
{
    const auto symbol = symbolFromString("DVAM1");
    CHECK(symbol == 211749525060);
}

TEST_CASE("SymbolToString :: Real conversion", "[parse-symbol]")
{
    const auto str = symbolToString(211749525060);
    CHECK(str == "DVAM1");
}

TEST_CASE("SymbolFromString :: Long string", "[parse-symbol]")
{
    CHECK_THROWS_WITH(symbolFromString("LONGSTRING"), Catch::Matchers::Contains("is to long"));
}
