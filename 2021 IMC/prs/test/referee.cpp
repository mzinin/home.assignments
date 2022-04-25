#include <player.hpp>
#include <referee.hpp>
#include <throw.hpp>
#include <types.hpp>

#include <catch2/catch.hpp>

#include <cassert>
#include <limits>
#include <sstream>

namespace
{
    class PlayerMock final : public Player
    {
    public:
        PlayerMock(std::string name, Score rounds, std::vector<Figure> throws)
            : mName{std::move(name)}
            , mRounds{rounds}
            , mThrows{throws}
        {
            assert(mThrows.size() >= static_cast<size_t>(mRounds));
        }

        Score numberOfRounds() const override
        {
            return mRounds;
        }

        Throw makeThrow() const override
        {
            return Throw{mThrows[mThrowIndex++]};
        }

        const std::string& name() const override
        {
            return mName;
        }

    private:
        std::string mName;
        Score mRounds;
        std::vector<Figure> mThrows;
        mutable size_t mThrowIndex = 0;
    };
}

TEST_CASE("Referee :: Number of Rounds", "[referee]")
{
    std::ostringstream output;
    Referee referee{output};

    SECTION("Equal number or rounds")
    {
        const Score expectedRounds = 3;
        const PlayerMock player1{"Player1", expectedRounds, {Figure::PAPER, Figure::ROCK, Figure::SCISSORS}};
        const PlayerMock player2{"Player2", expectedRounds, {Figure::PAPER, Figure::ROCK, Figure::SCISSORS}};

        const auto result = referee.playGame(player1, player2);
        CHECK(result.numberOfRounds == expectedRounds);
        CHECK(result.numberOfRounds == result.firstPlayerWins + result.secondPlayerWins + result.draws);
    }

    SECTION("First player wants more rounds")
    {
        const Score expectedRounds = 2;
        const PlayerMock player1{"Player1", expectedRounds + 1, {Figure::PAPER, Figure::ROCK, Figure::SCISSORS}};
        const PlayerMock player2{"Player2", expectedRounds, {Figure::PAPER, Figure::SCISSORS}};

        const auto result = referee.playGame(player1, player2);
        CHECK(result.numberOfRounds == expectedRounds);
        CHECK(result.numberOfRounds == result.firstPlayerWins + result.secondPlayerWins + result.draws);
    }

    SECTION("Second player wants more rounds")
    {
        const Score expectedRounds = 2;
        const PlayerMock player1{"Player1", expectedRounds, {Figure::PAPER, Figure::SCISSORS}};
        const PlayerMock player2{"Player2", expectedRounds + 1, {Figure::PAPER, Figure::ROCK, Figure::SCISSORS}};

        const auto result = referee.playGame(player1, player2);
        CHECK(result.numberOfRounds == expectedRounds);
        CHECK(result.numberOfRounds == result.firstPlayerWins + result.secondPlayerWins + result.draws);
    }
}

TEST_CASE("Referee :: Single Round Outcome", "[referee]")
{
    std::ostringstream output;
    Referee referee{output};

    SECTION("Draw")
    {
        const PlayerMock player1{"Player1", 1, {Figure::PAPER}};
        const PlayerMock player2{"Player2", 1, {Figure::PAPER}};

        const auto result = referee.playGame(player1, player2);
        CHECK(result.numberOfRounds == 1);
        CHECK(result.draws == 1);
    }

    SECTION("First player wins")
    {
        const PlayerMock player1{"Player1", 1, {Figure::PAPER}};
        const PlayerMock player2{"Player2", 1, {Figure::ROCK}};

        const auto result = referee.playGame(player1, player2);
        CHECK(result.numberOfRounds == 1);
        CHECK(result.firstPlayerWins == 1);
    }

    SECTION("Second player wins")
    {
        const PlayerMock player1{"Player1", 1, {Figure::PAPER}};
        const PlayerMock player2{"Player2", 1, {Figure::SCISSORS}};

        const auto result = referee.playGame(player1, player2);
        CHECK(result.numberOfRounds == 1);
        CHECK(result.secondPlayerWins == 1);
    }
}

TEST_CASE("Referee :: Multiple Rounds Outcome", "[referee]")
{
    std::ostringstream output;
    Referee referee{output};

    const PlayerMock player1{"Player1", 5, {Figure::PAPER, Figure::SCISSORS, Figure::ROCK, Figure::PAPER, Figure::SCISSORS}};
    const PlayerMock player2{"Player2", 5, {Figure::SCISSORS, Figure::PAPER, Figure::ROCK, Figure::SCISSORS, Figure::PAPER}};

    const auto result = referee.playGame(player1, player2);
    CHECK(result.numberOfRounds == 5);
    CHECK(result.firstPlayerWins == 2);
    CHECK(result.secondPlayerWins == 2);
    CHECK(result.draws == 1);
}

TEST_CASE("Referee :: Long Game", "[referee]")
{
    std::ostringstream output;
    Referee referee{output};

    const Score expectedRounds = std::min<uint64_t>(255, std::numeric_limits<Score>::max());
    const PlayerMock player1{"Player1", expectedRounds, std::vector(expectedRounds, Figure::PAPER)};
    const PlayerMock player2{"Player2", expectedRounds, std::vector(expectedRounds, Figure::ROCK)};

    const auto result = referee.playGame(player1, player2);
    CHECK(result.numberOfRounds == expectedRounds);
    CHECK(result.firstPlayerWins == expectedRounds);
    CHECK(result.secondPlayerWins == 0);
    CHECK(result.draws == 0);
}

TEST_CASE("Referee :: Single Player", "[referee]")
{
    std::ostringstream output;
    Referee referee{output};

    const PlayerMock player{"Player", 1, {Figure::PAPER}};

    CHECK_THROWS(referee.playGame(player, player));
}
