#include <iostream>

#include "application.h"
#include "mathhelper.h"

int main()
{
    try
    {
        Helper::randomSeed();

        Application app;

        app.run();
    }
    catch(std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    return EXIT_SUCCESS;
}
