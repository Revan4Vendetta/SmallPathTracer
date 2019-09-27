#ifdef _RANDOM_NUMBERS_H_
#define _RANDOM_NUMBERS_H_

#define _USE_MATH_DEFINES 
#include <math.h>
#include <cmath>

/* Random Number Generator */

namespace randomNumberGenerator
{

	class RandomGenerator
	{
	public:

		RandomGenerator() {};

		static double get_double() {
			double random = static_cast<double>(rand() / RAND_MAX);
			while (random >= 1.0f)
			{
				random = static_cast<double>(rand() / RAND_MAX);
			}
			return random;
		}


	};

}

#endif // _RANDOM_NUMBERS_H_
