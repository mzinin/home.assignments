#include <parse.hpp>

#include <catch2/catch.hpp>

#include <sstream>

TEST_CASE("RecordFromString :: Simple string", "[parse-record]")
{
    const auto record = recordFromString("15:30:00.000000;A;15;C;BUY;5;15.50");

    CHECK(record.ts == 55800000000);
    CHECK(record.symbol == 65);
    CHECK(record.orderID == 15);
    CHECK(record.operation == Operation::CANCEL);
    CHECK(record.side == Side::BUY);
    CHECK(record.volume == 5);
    CHECK(record.price == 155);
}

TEST_CASE("RecordFromString :: Real string", "[parse-record]")
{
    const auto record = recordFromString("15:48:46.630002;DVAM1;2870385;I;SELL;19;27.00");

    CHECK(record.ts == 56926630002);
    CHECK(record.symbol == 211749525060);
    CHECK(record.orderID == 2870385);
    CHECK(record.operation == Operation::INSERT);
    CHECK(record.side == Side::SELL);
    CHECK(record.volume == 19);
    CHECK(record.price == 270);
}

TEST_CASE("RecordFromString :: Negative price", "[parse-record]")
{
    const auto record = recordFromString("15:48:46.630002;DVAM1;2870385;I;SELL;19;-27.00");

    CHECK(record.ts == 56926630002);
    CHECK(record.symbol == 211749525060);
    CHECK(record.orderID == 2870385);
    CHECK(record.operation == Operation::INSERT);
    CHECK(record.side == Side::SELL);
    CHECK(record.volume == 19);
    CHECK(record.price == -270);
}

TEST_CASE("RecordFromString :: Wrong symbol", "[parse-record]")
{
    CHECK_THROWS_WITH(recordFromString("15:48:46.630002;DVAM123456;2870385;I;SELL;19;27.00"), Catch::Matchers::Contains("is to long"));
}

TEST_CASE("RecordFromString :: Wrong order ID", "[parse-record]")
{
    CHECK_THROWS_WITH(recordFromString("15:48:46.630002;DVAM1;-2870385;I;SELL;19;27.00"), Catch::Matchers::Contains("Wrong order ID value"));
}

TEST_CASE("RecordFromString :: Wrong operation", "[parse-record]")
{
    CHECK_THROWS_WITH(recordFromString("15:48:46.630002;DVAM1;2870385;X;SELL;19;27.00"), Catch::Matchers::Contains("Unsupported operation"));
}

TEST_CASE("RecordFromString :: Wrong side", "[parse-record]")
{
    CHECK_THROWS_WITH(recordFromString("15:48:46.630002;DVAM1;2870385;I;LOOK;19;27.00"), Catch::Matchers::Contains("Unsupported side"));
}

TEST_CASE("RecordFromString :: Wrong volume", "[parse-record]")
{
    CHECK_THROWS_WITH(recordFromString("15:48:46.630002;DVAM1;2870385;I;SELL;-19;27.00"), Catch::Matchers::Contains("Wrong volume value"));
}

TEST_CASE("RecordsFromStream :: Simple stream", "[parse-record]")
{
    std::istringstream stream;
    stream.str("15:30:00.000000;A;15;C;BUY;5;15.50\n"
               "15:30:01.000000;B;25;I;SELL;7;5.7\n");

    const auto records = recordsFromStream(stream);

    REQUIRE(records.size() == 2);

    CHECK(records[0].ts == 55800000000);
    CHECK(records[0].symbol == 65);
    CHECK(records[0].orderID == 15);
    CHECK(records[0].operation == Operation::CANCEL);
    CHECK(records[0].side == Side::BUY);
    CHECK(records[0].volume == 5);
    CHECK(records[0].price == 155);

    CHECK(records[1].ts == 55801000000);
    CHECK(records[1].symbol == 66);
    CHECK(records[1].orderID == 25);
    CHECK(records[1].operation == Operation::INSERT);
    CHECK(records[1].side == Side::SELL);
    CHECK(records[1].volume == 7);
    CHECK(records[1].price == 57);
}
