#ifndef __ELEGOO_INFRARED_CONFIG_H__
#define __ELEGOO_INFRARED_CONFIG_H__

#include "ElegooInfraredConfigInterface.h"

// Supporting the infrared remote control shipped with the car
class ElegooInfraredConfig: public ElegooInfraredConfigInterface
{
public:
	// May return UNKNOWN_CMD, will never return NO_COMMAND
	virtual ElegooCommand checkCommand(unsigned long possibleCommandCode)
	{
		switch (possibleCommandCode)
		{
		case 5316027:
		case 16736925:
			return ElegooCommand::MOVE_FORWARDS;
		case 16761405:
		case 553536955:
			return ElegooCommand::TURN_RIGHT;
		case 16720605:
		case 1386468383:
			return ElegooCommand::TURN_LEFT;
		case 16754775:
		case 2747854299:
			return ElegooCommand::MOVE_BACKWARDS;
		case 16712445:
		case 3622325019:
			return ElegooCommand::STOP_MOVING;
		default:
			return ElegooCommand::UNK_COMMAND;
		}
	}

	virtual ~ElegooInfraredConfig()
	{
	}
};

#endif
