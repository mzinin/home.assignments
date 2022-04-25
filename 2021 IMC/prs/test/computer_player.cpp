#include <computer_player.hpp>

#include <catch2/catch.hpp>

TEST_CASE("ComputerPlayer :: Custom Name", "[computer-player]")
{
    const std::string expectedName = "Machine";
    const ComputerPlayer player{expectedName};
    CHECK(expectedName == player.name());
}

TEST_CASE("ComputerPlayer :: Default Name", "[computer-player]")
{
    const ComputerPlayer player;
    CHECK(!player.name().empty());
}

TEST_CASE("ComputerPlayer :: Number of Rounds", "[computer-player]")
{
    const ComputerPlayer player;
    CHECK(player.numberOfRounds() > 0);
}

TEST_CASE("ComputerPlayer :: Make Throw", "[computer-player]")
{
    const ComputerPlayer player;
    for (int i = 0; i < 100; ++i)
    {
        const auto thrw = player.makeThrow();
        CHECK((thrw.value() == Figure::PAPER || thrw.value() == Figure::ROCK || thrw.value() == Figure::SCISSORS));
    }
}
