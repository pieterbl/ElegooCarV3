#ifndef __ELEGOO_MANUAL_DRIVER_H__
#define __ELEGOO_MANUAL_DRIVER_H__

#include "ElegooConstants.h"
#include "ElegooDriverBase.h"
#include "ElegooMotorUnit.h"

class ElegooManualDriver: public ElegooDriverBase
{
private:
	ElegooMotorUnit & motorUnit;

public:
	ElegooManualDriver(ElegooMotorUnit & pMotorUnit) :
			motorUnit(pMotorUnit)
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
			motorUnit.turnRight(250); // MS
			motorUnit.stopMoving();
			return ElegooConstants::OK;

		case ElegooCommand::TURN_RIGHT:
			motorUnit.turnRight(500); // MS
			motorUnit.stopMoving();
			return ElegooConstants::OK;

		case ElegooCommand::HALF_LEFT:
			motorUnit.turnLeft(250); // MS
			motorUnit.stopMoving();
			return ElegooConstants::OK;

		case ElegooCommand::TURN_LEFT:
			motorUnit.turnLeft(500); // MS
			motorUnit.stopMoving();
			return ElegooConstants::OK;

		default:
			return ElegooConstants::OK;
		}
	}
};

#endif
