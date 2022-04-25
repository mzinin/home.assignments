#include <computer_player.hpp>
#include <human_player.hpp>
#include <referee.hpp>

#include <cstdlib>
#include <iostream>

int main()
{
    HumanPlayer humanPlayer;
    ComputerPlayer computerPlayer;
    Referee referee;

    try
    {
        referee.playGame(humanPlayer, computerPlayer);
    }
    catch (const std::exception& e)
    {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
