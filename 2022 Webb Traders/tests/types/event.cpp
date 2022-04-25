#include "../../src/types/event.hpp"

#include <catch2/catch.hpp>

TEST_CASE("Types :: Event :: Parse insert")
{
    const std::string str = "INSERT,2,AAPL,BUY,14.235,6";
    const Event event{str};

    CHECK(event.command == Command::INSERT);
    CHECK(event.orderId == 2);
    CHECK(event.symbol == "AAPL");
    CHECK(event.side == Side::BUY);
    CHECK(event.price == Price{142350});
    CHECK(event.volume == 6);
}

TEST_CASE("Types :: Event :: Parse amend")
{
    const std::string str = "AMEND,23,46,3";
    const Event event{str};

    CHECK(event.command == Command::AMEND);
    CHECK(event.orderId == 23);
    CHECK(event.price == Price{460000});
    CHECK(event.volume == 3);
}

TEST_CASE("Types :: Event :: Parse pull")
{
    const std::string str = "PULL,1";
    const Event event{str};

    CHECK(event.command == Command::PULL);
    CHECK(event.orderId == 1);
}
