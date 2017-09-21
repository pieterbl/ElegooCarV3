#ifndef __ELEGOO_MATH_H__
#define __ELEGOO_MATH_H__

#include <Arduino.h>

class ElegooMath
{
public:
	static int distance(const int i1, const int i2)
	{
		int minVal = min(i1, i2);
		int maxVal = max(i1, i2);
		return (maxVal - minVal);
	}
};

#endif
