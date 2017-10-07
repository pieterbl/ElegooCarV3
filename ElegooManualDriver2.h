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
		if (checkFrontForObstacles(cmd) == ElegooConstants::STOPPED)
		{
			return motorUnit.statusOK();
		}

		int initiateMovementMS = 250;

		switch (cmd)
		{
		case ElegooCommand::MOVE_FORWARDS:
			return motorUnit.moveForwards(initiateMovementMS).statusOK();

		case ElegooCommand::MOVE_BACKWARDS:
			return motorUnit.moveBackwards(initiateMovementMS).statusOK();

		case ElegooCommand::HALF_RIGHT:
			return motorUnit.turnHalfRight(initiateMovementMS).statusOK();

		case ElegooCommand::TURN_RIGHT:
			return motorUnit.turnRight(initiateMovementMS).statusOK();

		case ElegooCommand::HALF_LEFT:
			return motorUnit.turnHalfLeft(initiateMovementMS).statusOK();

		case ElegooCommand::TURN_LEFT:
			return motorUnit.turnLeft(initiateMovementMS).statusOK();

		default:
			return motorUnit.statusOK();
		}
	}
};

#endif
