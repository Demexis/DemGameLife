#ifndef DEM_TIME_HPP
#define DEM_TIME_HPP

#include <ctime>

class DemTime
{
public:
	static float WaitForOneSecond();
	static float WaitForTime(float sec);
};

#endif