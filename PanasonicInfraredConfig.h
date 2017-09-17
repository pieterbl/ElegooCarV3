#ifndef __PANASONIC_INFRARED_CONFIG_H__
#define __PANASONIC_INFRARED_CONFIG_H__

#include "ElegooInfraredConfigInterface.h"

// Supporting the infrared remote control of my old radio ;-)
class PanasonicInfraredConfig: public ElegooInfraredConfigInterface
{
public:
	// May return UNKNOWN_CMD, will never return NO_COMMAND
	virtual ElegooCommand checkCommand(unsigned long possibleCommandCode)
	{
		switch (possibleCommandCode)
		{
		case 3810251948:
			return ElegooCommand::MOVE_FORWARDS;
		case 4025005874:
			return ElegooCommand::TURN_RIGHT;
		case 3039307748:
			return ElegooCommand::TURN_LEFT;
		case 2830352306:
			return ElegooCommand::MOVE_BACKWARDS;
		case 3416630992:
			return ElegooCommand::STOP_MOVING;
		default:
			return ElegooCommand::UNK_COMMAND;
		}
	}

	virtual ~PanasonicInfraredConfig()
	{
	}
};

#endif
