#ifndef __ELEGOO_INFRARED_CONFIG_H__
#define __ELEGOO_INFRARED_CONFIG_H__

#include "ElegooInfraredConfigInterface.h"

class ElegooInfraredConfig: public ElegooInfraredConfigInterface
{
public:
	virtual ElegooMoveCommand checkCommand(unsigned long possibleCommandCode)
	{
		switch (possibleCommandCode)
		{
		case 5316027:
		case 16736925:
			return ElegooMoveCommand::MOVE_FORWARDS;
		case 16761405:
		case 553536955:
			return ElegooMoveCommand::TURN_RIGHT;
		case 16720605:
		case 1386468383:
			return ElegooMoveCommand::TURN_LEFT;
		case 16754775:
		case 2747854299:
			return ElegooMoveCommand::MOVE_BACKWARDS;
		case 16712445:
		case 3622325019:
			return ElegooMoveCommand::STOP_MOVING;
		default:
			return ElegooMoveCommand::UNKNOWN_CMD;
		}
	}

	virtual ~ElegooInfraredConfig()
	{
	}
};

#endif
