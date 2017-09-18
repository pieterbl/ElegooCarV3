#ifndef __ELEGOO_AUTOMATIC_DRIVER_2_H__
#define __ELEGOO_AUTOMATIC_DRIVER_2_H__

#include "ElegooDriverBase.h"

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
		adjustPositionForDirection(biggestDistance.direction);

		if (biggestDistance.distance < safetyDistanceInCM)
		{
			return backOut();
		}

		int milliSeconds = biggestDistance.distance * 10;
		if (milliSeconds > 3000)
		{
			milliSeconds = 3000;
		}

		motorUnit.moveForwards(milliSeconds);
		motorUnit.stopMoving();
		return ElegooConstants::OK;
	}

private:
	int adjustPositionForDirection(int direction) // TODO MotorUnit operations to return reference to MotorUnit
	{
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
		return ElegooConstants::OK;
	}
};

#endif
