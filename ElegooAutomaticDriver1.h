#ifndef __ELEGOO_AUTOMATIC_DRIVER_1_H__
#define __ELEGOO_AUTOMATIC_DRIVER_1_H__

#include "ElegooDriverBase.h"

// is interruptible
class ElegooAutomaticDriver1: public ElegooDriverBase
{
public:
	ElegooAutomaticDriver1(int pSafetyDistanceInCM, ElegooDistanceUnit & pDistUnit, ElegooMotorUnit & pMotorUnit) :
			ElegooDriverBase(pSafetyDistanceInCM, pDistUnit, pMotorUnit)
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
			return motorUnit.moveForwards().statusOK();
		}

		// frontDistance <= safetyDistanceInCM !!
		motorUnit.stopMoving();

		const int rightDistance = distUnit.rightDistance();
		const int leftDistance = distUnit.leftDistance();
		distUnit.frontDistance(); // reposition sensor, to avoid delays

		if ((rightDistance > safetyDistanceInCM) && (rightDistance >= leftDistance))
		{
			return motorUnit.turnRight().stopMoving().statusOK();
		}

		if ((leftDistance > safetyDistanceInCM) && leftDistance >= rightDistance)
		{
			return motorUnit.turnLeft().stopMoving().statusOK();
		}

		// we're stuck
		return backOut().statusOK();
	}

};

#endif
