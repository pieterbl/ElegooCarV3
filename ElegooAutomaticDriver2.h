#ifndef __ELEGOO_AUTOMATIC_DRIVER_2_H__
#define __ELEGOO_AUTOMATIC_DRIVER_2_H__

#include "ElegooDriverBase.h"

// is interruptible
class ElegooAutomaticDriver2: public ElegooDriverBase
{
public:
	ElegooAutomaticDriver2(int pSafetyDistanceInCM, ElegooDistanceUnit & pDistUnit, ElegooMotorUnit & pMotorUnit) :
			ElegooDriverBase(pSafetyDistanceInCM, pDistUnit, pMotorUnit)
	{
	}

	virtual ~ElegooAutomaticDriver2()
	{
	}

	virtual int processCommand(ElegooCommand cmd)
	{
		DistanceData biggestDistance = distUnit.scanBiggestDistance();
		distUnit.frontDistance(); // position "eyes" to the front
		adjustPositionForDirection(biggestDistance.direction);

		if (biggestDistance.distance < safetyDistanceInCM)
		{
			return backOut().statusOK();
		}

		int milliSeconds = biggestDistance.distance * 10;
		if (milliSeconds > 3000)
		{
			milliSeconds = 3000;
		}

		return motorUnit.moveForwards(milliSeconds).stopMoving().statusOK();
	}

private:
	// is interruptible
	ElegooMotorUnit & adjustPositionForDirection(int direction)
	{
		if (motorUnit.hasCommand())
		{
			return motorUnit;
		}

		if (direction == ElegooDistanceUnit::HALF_LEFT)
		{
			return motorUnit.turnHalfLeft();
		}
		if (direction == ElegooDistanceUnit::HALF_RIGHT)
		{
			return motorUnit.turnHalfRight();
		}
		if (distUnit.isFarLeftDirection(direction))
		{
			return motorUnit.turnLeft();
		}
		if (distUnit.isFarRightDirection(direction))
		{
			return motorUnit.turnRight();
		}

		return motorUnit;
	}
};

#endif
