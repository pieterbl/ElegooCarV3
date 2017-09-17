#ifndef __ELEGOO_MANUAL_DRIVER_H__
#define __ELEGOO_MANUAL_DRIVER_H__

#include "ElegooConstants.h"
#include "ElegooDriverBase.h"

class ElegooManualDriver: public ElegooDriverBase
{
private:
	ElegooMotorUnit & motorUnit;

public:
	virtual ElegooManualDriver(ElegooMotorUnit & pMotorUnit) :
			motorUnit(pMotorUnit)
	{
	}

	virtual ~ElegooManualDriver()
	{
	}

	virtual int processCommand(ElegooMoveCommand command)
	{
		switch (command)
		{
		case ElegooMoveCommand::MOVE_FORWARDS:
			motorUnit.moveForwards(1000);
			motorUnit.stopMoving();
			return ElegooConstants::OK;

		case ElegooMoveCommand::MOVE_BACKWARDS:
			motorUnit.moveBackwards();
			motorUnit.stopMoving();
			return ElegooConstants::OK;

		case ElegooMoveCommand::HALF_RIGHT:
			motorUnit.turnRight(250); // MS
			motorUnit.stopMoving();
			return ElegooConstants::OK;

		case ElegooMoveCommand::TURN_RIGHT:
			motorUnit.turnRight(500); // MS
			motorUnit.stopMoving();
			return ElegooConstants::OK;

		case ElegooMoveCommand::HALF_LEFT:
			motorUnit.turnLeft(250); // MS
			motorUnit.stopMoving();
			return ElegooConstants::OK;

		case ElegooMoveCommand::TURN_LEFT:
			motorUnit.turnLeft(500); // MS
			motorUnit.stopMoving();
			return ElegooConstants::OK;
		}

		return ElegooConstants::OK;
	}
};

#endif
