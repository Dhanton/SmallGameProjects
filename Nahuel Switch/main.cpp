#include <iostream>

#include "stagemanager.h"
#include "game.h"
#include "mathhelper.h"

int main()
{
   try
    {
        Helper::randomSeed();

        StageManager game;

        game.run();

        return EXIT_SUCCESS;
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_FAILURE;
}
