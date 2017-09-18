#ifndef __ELEGOO_AUTOMATIC_DRIVER_1_H__
#define __ELEGOO_AUTOMATIC_DRIVER_1_H__

#include "ElegooConstants.h"
#include "ElegooDriverBase.h"
#include "ElegooDistanceUnit.h"
#include "ElegooMotorUnit.h"

class ElegooAutomaticDriver1: public ElegooDriverBase
{
private:
	int safetyDistanceInCM;
	ElegooDistanceUnit & distUnit;
	ElegooMotorUnit & motorUnit;

public:
	ElegooAutomaticDriver1( //
			int pSafetyDistanceInCM, ElegooDistanceUnit & pDistUnit, ElegooMotorUnit & pMotorUnit) :
			safetyDistanceInCM(pSafetyDistanceInCM), distUnit(pDistUnit), motorUnit(pMotorUnit)
	{
	}

	virtual ~ElegooAutomaticDriver1()
	{
	}

	virtual int processCommand(ElegooCommand cmd)
	{
		const int frontDistance = distUnit.frontDistance();
		if (frontDistance > safetyDistanceInCM)
		{
			return motorUnit.moveForwards();
		}

		// frontDistance <= safetyDistanceInCM !!
		motorUnit.stopMoving();

		const int rightDistance = distUnit.rightDistance();
		const int leftDistance = distUnit.leftDistance();
		distUnit.frontDistance(); // reposition sensor, to avoid delays

		if ((rightDistance > safetyDistanceInCM) && (rightDistance >= leftDistance))
		{
			motorUnit.turnRight();
			motorUnit.stopMoving();
			return ElegooConstants::OK;
		}

		if ((leftDistance > safetyDistanceInCM) && leftDistance >= rightDistance)
		{
			motorUnit.turnLeft();
			motorUnit.stopMoving();
			return ElegooConstants::OK;
		}

		// we're stuck
		return backOut();
	}

private:

	// TODO the backOut routine cannot be interrupted by a button press or so, this must be fixed (note: this routine is very unlikely to get called)
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
