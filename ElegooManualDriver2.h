#ifndef __ELEGOO_MANUAL_DRIVER_2_H__
#define __ELEGOO_MANUAL_DRIVER_2_H__

#include "ElegooDriverBase.h"

// is interruptible
class ElegooManualDriver2: public ElegooDriverBase
{
public:
	ElegooManualDriver2(int pSafetyDistanceInCM, ElegooDistanceUnit & pDistUnit, ElegooMotorUnit & pMotorUnit) :
			ElegooDriverBase(pSafetyDistanceInCM, pDistUnit, pMotorUnit)
	{
	}

	virtual ~ElegooManualDriver2()
	{
	}

	virtual int processCommand(ElegooCommand cmd)
	{
		// check front sensor and stop
		const int frontDistance = distUnit.frontDistance();
		if (frontDistance < safetyDistanceInCM)
		{
			return motorUnit.stopMoving().statusOK();
		}

		switch (cmd)
		{
		case ElegooCommand::MOVE_FORWARDS:
			return motorUnit.moveForwards().statusOK();

		case ElegooCommand::MOVE_BACKWARDS:
			return motorUnit.moveBackwards().statusOK();

		case ElegooCommand::HALF_RIGHT:
			return motorUnit.turnHalfRight().statusOK();

		case ElegooCommand::TURN_RIGHT:
			return motorUnit.turnRight().statusOK();

		case ElegooCommand::HALF_LEFT:
			return motorUnit.turnHalfLeft().statusOK();

		case ElegooCommand::TURN_LEFT:
			return motorUnit.turnLeft().statusOK();

		default:
			return motorUnit.statusOK();
		}
	}
};

#endif
