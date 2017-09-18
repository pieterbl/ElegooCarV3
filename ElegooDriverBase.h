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

	// TODO (LOW) the backOut routine cannot be interrupted by a button press or so, this must be fixed (note: this routine is very unlikely to get called)
	int backOut()
	{
		bool doBackOut = true;
		int rightDistance = 0;
		int leftDistance = 0;

		do
		{
			motorUnit.moveBackwards();
			motorUnit.stopMoving();
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
