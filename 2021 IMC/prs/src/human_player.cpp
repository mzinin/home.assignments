#include <human_player.hpp>

#include <algorithm>
#include <limits>
#include <stdexcept>

namespace
{
    constexpr int64_t MIN_ROUNDS = 0;
    constexpr int64_t MAX_ROUNDS = std::numeric_limits<Score>::max();

    const std::string DEFAULT_HUMAN_NAME = "Human";
    const std::string PAPER_SHORT = "P";
    const std::string PAPER_LONG = "PAPER";
    const std::string ROCK_SHORT = "R";
    const std::string ROCK_LONG = "ROCK";
    const std::string SCISSORS_SHORT = "S";
    const std::string SCISSORS_LONG = "SCISSORS";
}

HumanPlayer::HumanPlayer()
    : HumanPlayer{std::cin, std::cout}
{
}

HumanPlayer::HumanPlayer(std::istream& input, std::ostream& output)
    : mInput{input}
    , mOutput{output}
{
}

Score HumanPlayer::numberOfRounds() const
{
    int64_t rounds = 0;

    while (true)
    {
        mOutput << "How many rounds would you like to play? Enter a number between " << MIN_ROUNDS << " and " << MAX_ROUNDS << ":\n";

        std::string answer;
        std::getline(mInput, answer);

        try
        {
            rounds = std::stoull(answer);
            if (rounds < MIN_ROUNDS)
            {
                mOutput << "Number of rounds is too small, try again." << std::endl;
            }
            else if (rounds > MAX_ROUNDS)
            {
                mOutput << "Number of rounds is too big, try again." << std::endl;
            }
            else
            {
                break;
            }
        }
        catch (const std::invalid_argument&)
        {
            mOutput << "Not a number, try again." << std::endl;
        }
        catch (const std::out_of_range&)
        {
            mOutput << "Wrong number of rounds, try again." << std::endl;
        }
    }

    return static_cast<Score>(rounds);
}

Throw HumanPlayer::makeThrow() const
{
    Figure figure;

    while (true)
    {
        mOutput << "Choose your throw (P[APER], R[OCK], S[CISSORS]):\n";

        std::string choice;
        std::getline(mInput, choice);
        std::for_each(choice.begin(), choice.end(), [](auto& c){ c = std::toupper(c); });

        if (choice == PAPER_SHORT || choice == PAPER_LONG)
        {
            figure = Figure::PAPER;
            break;
        }
        else if (choice == ROCK_SHORT || choice == ROCK_LONG)
        {
            figure = Figure::ROCK;
            break;
        }
        else if (choice == SCISSORS_SHORT || choice == SCISSORS_LONG)
        {
            figure = Figure::SCISSORS;
            break;
        }
        else
        {
            mOutput << "Invalid choice, try again." << std::endl;
        }
    }

    return Throw{figure};
}

const std::string& HumanPlayer::name() const
{
    // TODO ask human
    return DEFAULT_HUMAN_NAME;
}