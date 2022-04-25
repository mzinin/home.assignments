#include <throw.hpp>

#include <catch2/catch.hpp>

#include <sstream>

TEST_CASE("Throw :: Create", "[throw]")
{
    SECTION("PAPER")
    {
        const auto thrw = Throw{Figure::PAPER};
        CHECK(thrw.value() == Figure::PAPER);
    }

    SECTION("ROCK")
    {
        const auto thrw = Throw{Figure::ROCK};
        CHECK(thrw.value() == Figure::ROCK);
    }

    SECTION("SCISSORS")
    {
        const auto thrw = Throw{Figure::SCISSORS};
        CHECK(thrw.value() == Figure::SCISSORS);
    }
}

TEST_CASE("Throw :: Compare", "[throw]")
{
    const auto throwPaper = Throw{Figure::PAPER};
    const auto throwRock = Throw{Figure::ROCK};
    const auto throwScissors = Throw{Figure::SCISSORS};
    const auto throwPaper2 = Throw{Figure::PAPER};

    CHECK(throwPaper.compare(throwRock) == Outcome::WIN);
    CHECK(throwPaper.compare(throwScissors) == Outcome::DEFEAT);
    CHECK(throwPaper.compare(throwPaper2) == Outcome::DRAW);
    CHECK(throwPaper.compare(throwPaper) == Outcome::DRAW);

    CHECK(throwRock.compare(throwPaper) == Outcome::DEFEAT);
    CHECK(throwRock.compare(throwScissors) == Outcome::WIN);
    CHECK(throwRock.compare(throwRock) == Outcome::DRAW);

    CHECK(throwScissors.compare(throwPaper) == Outcome::WIN);
    CHECK(throwScissors.compare(throwRock) == Outcome::DEFEAT);
    CHECK(throwScissors.compare(throwScissors) == Outcome::DRAW);
}

TEST_CASE("Throw :: Print out", "[throw]")
{
    std::ostringstream output;

    SECTION("PAPER")
    {
        const auto thrw = Throw{Figure::PAPER};
        output << thrw;
        CHECK(output.str() == "PAPER");
    }

    SECTION("ROCK")
    {
        const auto thrw = Throw{Figure::ROCK};
        output << thrw;
        CHECK(output.str() == "ROCK");
    }

    SECTION("SCISSORS")
    {
        const auto thrw = Throw{Figure::SCISSORS};
        output << thrw;
        CHECK(output.str() == "SCISSORS");
    }
}
