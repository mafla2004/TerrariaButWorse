#ifndef __MATHFUNCS__
#define __MATHFUNCS__

namespace math
{
	constexpr double PI			= 3.141592653;
	constexpr double TAU		= 2 * PI;
	constexpr double PI_HALVES	= PI / 2;
	constexpr double PI_FOURTHS = PI / 4;
	constexpr double PI_THIRDS	= PI / 3;
	constexpr double PI_SIXTHS	= PI / 6;

	inline constexpr double module(double a, double b) { return a - b * (int)(a / b); }

	inline constexpr double toDeg(double rad) { return (rad / PI) * 180; }
	inline constexpr double toRad(double deg) { return (deg / 180) * PI; }

	inline constexpr double cos(double rad)
	{
		double x = module(rad, PI); // Module
		double x2 = x * x;
		double x4 = x2 * x2;
		double x6 = x2 * x4;
		
		return 1 - 0.5 * x2 + (1 / 24) * x4 - (1 / 720) * x6;
	}
}

#endif