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
			return motorUnit.moveForwards(1000).stopMoving().statusOK();

		case ElegooCommand::MOVE_BACKWARDS:
			return motorUnit.moveBackwards().stopMoving().statusOK();

		case ElegooCommand::HALF_RIGHT:
			return motorUnit.turnHalfRight().stopMoving().statusOK();

		case ElegooCommand::TURN_RIGHT:
			return motorUnit.turnRight().stopMoving().statusOK();

		case ElegooCommand::HALF_LEFT:
			return motorUnit.turnHalfLeft().stopMoving().statusOK();

		case ElegooCommand::TURN_LEFT:
			return motorUnit.turnLeft().stopMoving().statusOK();

		default:
			return motorUnit.statusOK();
		}
	}
};

#endif
