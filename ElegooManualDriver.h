#ifndef __ELEGOO_MANUAL_DRIVER_H__
#define __ELEGOO_MANUAL_DRIVER_H__

#include "ElegooDriverBase.h"

class ElegooManualDriver: public ElegooDriverBase
{
public:
	ElegooManualDriver(int pSafetyDistanceInCM, ElegooDistanceUnit & pDistUnit, ElegooMotorUnit & pMotorUnit) :
			ElegooDriverBase(pSafetyDistanceInCM, pDistUnit, pMotorUnit)
	{
	}

	virtual ~ElegooManualDriver()
	{
	}

	virtual int processCommand(ElegooCommand cmd)
	{
		switch (cmd)
		{
		case ElegooCommand::MOVE_FORWARDS:
			motorUnit.moveForwards(1000);
			motorUnit.stopMoving();
			return ElegooConstants::OK;

		case ElegooCommand::MOVE_BACKWARDS:
			motorUnit.moveBackwards();
			motorUnit.stopMoving();
			return ElegooConstants::OK;

		case ElegooCommand::HALF_RIGHT:
			motorUnit.turnHalfRight();
			motorUnit.stopMoving();
			return ElegooConstants::OK;

		case ElegooCommand::TURN_RIGHT:
			motorUnit.turnRight();
			motorUnit.stopMoving();
			return ElegooConstants::OK;

		case ElegooCommand::HALF_LEFT:
			motorUnit.turnHalfLeft();
			motorUnit.stopMoving();
			return ElegooConstants::OK;

		case ElegooCommand::TURN_LEFT:
			motorUnit.turnLeft();
			motorUnit.stopMoving();
			return ElegooConstants::OK;

		default:
			return ElegooConstants::OK;
		}
	}
};

#endif
