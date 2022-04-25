#include <human_player.hpp>

#include <catch2/catch.hpp>

#include <sstream>

namespace
{
    const std::string HOW_MANY_ROUNDS = "How many rounds";
    const std::string TRY_AGAIN = "try again";
    const std::string NOT_A_NUMBER = "Not a number";
    const std::string IS_TOO_SMALL = "is too small";
    const std::string IS_TOO_BIG = "is too big";
    const std::string CHOOSE_THROW = "Choose your throw";
}

TEST_CASE("HumanPlayer :: Number of Rounds", "[human-player]")
{
    std::istringstream input;
    std::ostringstream output;
    const HumanPlayer player{input, output};

    SECTION("Correct Number")
    {
        const auto expectedNumber = 14;
        input.str(std::to_string(expectedNumber));
        const auto actualNumber = player.numberOfRounds();

        CHECK(actualNumber == expectedNumber);
        CHECK(output.str().find(HOW_MANY_ROUNDS) != std::string::npos);
        CHECK(output.str().find(HOW_MANY_ROUNDS) == output.str().rfind(HOW_MANY_ROUNDS));
        CHECK(output.str().find(TRY_AGAIN) == std::string::npos);
    }

    SECTION("Not a Number")
    {
        const auto expectedNumber = 15;
        input.str("abc\n" + std::to_string(expectedNumber));
        const auto actualNumber = player.numberOfRounds();

        CHECK(actualNumber == expectedNumber);
        CHECK(output.str().find(HOW_MANY_ROUNDS) != std::string::npos);
        CHECK(output.str().find(HOW_MANY_ROUNDS) != output.str().rfind(HOW_MANY_ROUNDS));
        CHECK(output.str().find(TRY_AGAIN) != std::string::npos);
        CHECK(output.str().find(NOT_A_NUMBER) != std::string::npos);
    }

    SECTION("Small Number")
    {
        const auto smallNumber = -2;
        const auto expectedNumber = 16;
        input.str(std::to_string(smallNumber) + "\n" + std::to_string(expectedNumber));
        const auto actualNumber = player.numberOfRounds();

        CHECK(actualNumber == expectedNumber);
        CHECK(output.str().find(HOW_MANY_ROUNDS) != std::string::npos);
        CHECK(output.str().find(HOW_MANY_ROUNDS) != output.str().rfind(HOW_MANY_ROUNDS));
        CHECK(output.str().find(TRY_AGAIN) != std::string::npos);
        CHECK(output.str().find(IS_TOO_SMALL) != std::string::npos);
    }

    SECTION("Big Number")
    {
        const auto bigNumber = 1234567890ul;
        const auto expectedNumber = 17;
        input.str(std::to_string(bigNumber) + "\n" + std::to_string(expectedNumber));
        const auto actualNumber = player.numberOfRounds();

        CHECK(actualNumber == expectedNumber);
        CHECK(output.str().find(HOW_MANY_ROUNDS) != std::string::npos);
        CHECK(output.str().find(HOW_MANY_ROUNDS) != output.str().rfind(HOW_MANY_ROUNDS));
        CHECK(output.str().find(TRY_AGAIN) != std::string::npos);
        CHECK(output.str().find(IS_TOO_BIG) != std::string::npos);
    }
}

TEST_CASE("HumanPlayer :: Make Throw", "[human-player]")
{
    std::istringstream input;
    std::ostringstream output;
    const HumanPlayer player{input, output};

    SECTION("Paper Short Lowercase")
    {
        input.str("p");
        const auto thrw = player.makeThrow();

        CHECK(thrw.value() == Figure::PAPER);
        CHECK(output.str().find(CHOOSE_THROW) != std::string::npos);
        CHECK(output.str().find(CHOOSE_THROW) == output.str().rfind(CHOOSE_THROW));
        CHECK(output.str().find(TRY_AGAIN) == std::string::npos);
    }

    SECTION("Paper Long Lowercase")
    {
        input.str("paper");
        const auto thrw = player.makeThrow();

        CHECK(thrw.value() == Figure::PAPER);
        CHECK(output.str().find(CHOOSE_THROW) != std::string::npos);
        CHECK(output.str().find(CHOOSE_THROW) == output.str().rfind(CHOOSE_THROW));
        CHECK(output.str().find(TRY_AGAIN) == std::string::npos);
    }

    SECTION("Rock Short Uppercase")
    {
        input.str("R");
        const auto thrw = player.makeThrow();

        CHECK(thrw.value() == Figure::ROCK);
        CHECK(output.str().find(CHOOSE_THROW) != std::string::npos);
        CHECK(output.str().find(CHOOSE_THROW) == output.str().rfind(CHOOSE_THROW));
        CHECK(output.str().find(TRY_AGAIN) == std::string::npos);
    }

    SECTION("Rock Long Uppercase")
    {
        input.str("ROCK");
        const auto thrw = player.makeThrow();

        CHECK(thrw.value() == Figure::ROCK);
        CHECK(output.str().find(CHOOSE_THROW) != std::string::npos);
        CHECK(output.str().find(CHOOSE_THROW) == output.str().rfind(CHOOSE_THROW));
        CHECK(output.str().find(TRY_AGAIN) == std::string::npos);
    }

    SECTION("Scissors Short Lowercase")
    {
        input.str("s");
        const auto thrw = player.makeThrow();

        CHECK(thrw.value() == Figure::SCISSORS);
        CHECK(output.str().find(CHOOSE_THROW) != std::string::npos);
        CHECK(output.str().find(CHOOSE_THROW) == output.str().rfind(CHOOSE_THROW));
        CHECK(output.str().find(TRY_AGAIN) == std::string::npos);
    }

    SECTION("Scissors Long Uppercase")
    {
        input.str("SCISSORS");
        const auto thrw = player.makeThrow();

        CHECK(thrw.value() == Figure::SCISSORS);
        CHECK(output.str().find(CHOOSE_THROW) != std::string::npos);
        CHECK(output.str().find(CHOOSE_THROW) == output.str().rfind(CHOOSE_THROW));
        CHECK(output.str().find(TRY_AGAIN) == std::string::npos);
    }

    SECTION("Wrong choice")
    {
        input.str("r p\ns");
        const auto thrw = player.makeThrow();

        CHECK(thrw.value() == Figure::SCISSORS);
        CHECK(output.str().find(CHOOSE_THROW) != std::string::npos);
        CHECK(output.str().find(CHOOSE_THROW) != output.str().rfind(CHOOSE_THROW));
        CHECK(output.str().find(TRY_AGAIN) != std::string::npos);
    }
}
