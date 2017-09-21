#ifndef __ELEGOO_DRIVER_BASE_H__
#define __ELEGOO_DRIVER_BASE_H__

#include "ElegooConstants.h"
#include "ElegooDistanceUnit.h"
#include "ElegooMotorUnit.h"

class ElegooDriverBase
{
protected:
	int safetyDistanceInCM;
	ElegooDistanceUnit & distUnit;
	ElegooMotorUnit & motorUnit;

public:
	ElegooDriverBase(int pSafetyDistanceInCM, ElegooDistanceUnit & pDistUnit, ElegooMotorUnit & pMotorUnit) :
			safetyDistanceInCM(pSafetyDistanceInCM), distUnit(pDistUnit), motorUnit(pMotorUnit)
	{
	}

	virtual ~ElegooDriverBase()
	{
	}

	virtual int processCommand(ElegooCommand cmd) = 0;

protected:

	ElegooMotorUnit & backOut()
	{
		bool doBackOut = true;
		int rightDistance = 0;
		int leftDistance = 0;

		do
		{
			motorUnit.moveBackwards().stopMoving();
			rightDistance = distUnit.rightDistance();
			leftDistance = distUnit.leftDistance();

			doBackOut = (rightDistance <= safetyDistanceInCM) && (leftDistance <= safetyDistanceInCM);

		} while (doBackOut);

		// here either (rightDistance > safetyDistanceInCM) || (leftDistance > safetyDistanceInCM)
		if (rightDistance > safetyDistanceInCM)
		{
			return motorUnit.turnRight();
		}
		else
		{
			return motorUnit.turnLeft();
		}
	}
};

#endif
