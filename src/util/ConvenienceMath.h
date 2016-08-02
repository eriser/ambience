#ifndef CONVENIENCEMATH_H
#define CONVENIENCEMATH_H

#include <cmath>

namespace ambience
{
namespace util
{

// Returns a random number in the paased interval
real randomRealInterval(real min, real max)
{
	return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}

int randomIntInterval(int min, int max)
{
	return min + (rand() % (int)(max - min + 1));
}

bool randomBool()
{
	return randomRealInterval(0.0, 1.0) < 0.5;
}

}
}

#endif