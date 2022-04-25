#include "../../src/engine/order_batch.hpp"

#include <catch2/catch.hpp>

TEST_CASE("Engine :: Batch :: Empty")
{
    OrderBatch batch;

    CHECK(batch.empty());
    CHECK(batch.totalVolume() == 0);
}

TEST_CASE("Engine :: Batch :: Add one order")
{
    OrderBatch batch;
    CHECK(batch.add(1, 100));

    CHECK(!batch.empty());
    CHECK(batch.totalVolume() == 100);
    CHECK(batch.topOrder().id == 1);
    CHECK(batch.topOrder().volume == 100);
}

TEST_CASE("Engine :: Batch :: Add two orders")
{
    OrderBatch batch;
    CHECK(batch.add(1, 100));
    CHECK(batch.add(2, 200));

    CHECK(!batch.empty());
    CHECK(batch.totalVolume() == 300);
    CHECK(batch.topOrder().id == 1);
    CHECK(batch.topOrder().volume == 100);
}

TEST_CASE("Engine :: Batch :: Add the same order")
{
    OrderBatch batch;
    CHECK(batch.add(1, 100));
    CHECK(!batch.add(1, 200));

    CHECK(!batch.empty());
    CHECK(batch.totalVolume() == 100);
    CHECK(batch.topOrder().id == 1);
    CHECK(batch.topOrder().volume == 100);
}

TEST_CASE("Engine :: Batch :: Erase first order")
{
    OrderBatch batch;
    CHECK(batch.add(1, 100));
    CHECK(batch.add(2, 200));
    CHECK(batch.add(3, 300));
    CHECK(batch.erase(1));

    CHECK(!batch.empty());
    CHECK(batch.totalVolume() == 500);
    CHECK(batch.topOrder().id == 2);
    CHECK(batch.topOrder().volume == 200);
}

TEST_CASE("Engine :: Batch :: Erase middle order")
{
    OrderBatch batch;
    CHECK(batch.add(1, 100));
    CHECK(batch.add(2, 200));
    CHECK(batch.add(3, 300));
    CHECK(batch.erase(2));

    CHECK(!batch.empty());
    CHECK(batch.totalVolume() == 400);
    CHECK(batch.topOrder().id == 1);
    CHECK(batch.topOrder().volume == 100);
}

TEST_CASE("Engine :: Batch :: Erase last order")
{
    OrderBatch batch;
    CHECK(batch.add(1, 100));
    CHECK(batch.add(2, 200));
    CHECK(batch.add(3, 300));
    CHECK(batch.erase(3));

    CHECK(!batch.empty());
    CHECK(batch.totalVolume() == 300);
    CHECK(batch.topOrder().id == 1);
    CHECK(batch.topOrder().volume == 100);
}

TEST_CASE("Engine :: Batch :: Erase wrong order")
{
    OrderBatch batch;
    CHECK(batch.add(1, 100));
    CHECK(batch.add(2, 200));
    CHECK(batch.add(3, 300));
    CHECK(!batch.erase(4));

    CHECK(!batch.empty());
    CHECK(batch.totalVolume() == 600);
    CHECK(batch.topOrder().id == 1);
    CHECK(batch.topOrder().volume == 100);
}

TEST_CASE("Engine :: Batch :: Increase volume by id")
{
    OrderBatch batch;
    CHECK(batch.add(1, 100));
    CHECK(batch.add(2, 200));
    CHECK(batch.updateVolume(1, 300));

    CHECK(!batch.empty());
    CHECK(batch.totalVolume() == 500);
    CHECK(batch.topOrder().id == 2);
    CHECK(batch.topOrder().volume == 200);
}

TEST_CASE("Engine :: Batch :: Decrease volume by id")
{
    OrderBatch batch;
    CHECK(batch.add(1, 100));
    CHECK(batch.add(2, 200));
    CHECK(batch.updateVolume(1, 50));

    CHECK(!batch.empty());
    CHECK(batch.totalVolume() == 250);
    CHECK(batch.topOrder().id == 1);
    CHECK(batch.topOrder().volume == 50);
}

TEST_CASE("Engine :: Batch :: Decrease volume by id to zero")
{
    OrderBatch batch;
    CHECK(batch.add(1, 100));
    CHECK(batch.updateVolume(1, 0));

    CHECK(batch.empty());
    CHECK(batch.totalVolume() == 0);
}

TEST_CASE("Engine :: Batch :: Decrease volume by ref")
{
    OrderBatch batch;
    CHECK(batch.add(1, 100));
    CHECK(batch.add(2, 200));
    CHECK(batch.updateVolume(batch.topOrder(), 50));

    CHECK(!batch.empty());
    CHECK(batch.totalVolume() == 250);
    CHECK(batch.topOrder().id == 1);
    CHECK(batch.topOrder().volume == 50);
}

TEST_CASE("Engine :: Batch :: Decrease volume by ref to zero")
{
    OrderBatch batch;
    CHECK(batch.add(1, 100));
    CHECK(batch.add(2, 200));
    CHECK(batch.updateVolume(batch.topOrder(), 0));

    CHECK(!batch.empty());
    CHECK(batch.totalVolume() == 200);
    CHECK(batch.topOrder().id == 2);
    CHECK(batch.topOrder().volume == 200);
}
