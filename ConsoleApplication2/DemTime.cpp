#include "DemTime.hpp"

float DemTime::WaitForOneSecond()
{
    clock_t oldTime = clock(), newTime = clock();

    while (newTime - oldTime < CLOCKS_PER_SEC)
    {
        newTime = clock();
    }

    return 1.0f;
}

float DemTime::WaitForTime(float sec)
{
    clock_t oldTime = clock(), newTime = clock();

    while (newTime - oldTime < (int)(CLOCKS_PER_SEC * sec))
    {
        newTime = clock();
    }

    return sec;
}