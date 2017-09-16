#ifndef __PANASONIC_INFRARED_CONFIG_H__
#define __PANASONIC_INFRARED_CONFIG_H__

#include "ElegooInfraredConfigInterface.h"

// Supporting the infrared remote control of my old radio ;-)
class PanasonicInfraredConfig: public ElegooInfraredConfigInterface
{
public:
	virtual ElegooMoveCommand checkCommand(unsigned long possibleCommandCode)
	{
		switch (possibleCommandCode)
		{
		case 3810251948:
			return ElegooMoveCommand::MOVE_FORWARDS;
		case 4025005874:
			return ElegooMoveCommand::TURN_RIGHT;
		case 3039307748:
			return ElegooMoveCommand::TURN_LEFT;
		case 2830352306:
			return ElegooMoveCommand::MOVE_BACKWARDS;
		case 3416630992:
			return ElegooMoveCommand::STOP_MOVING;
		default:
			return ElegooMoveCommand::UNKNOWN_CMD;
		}
	}

	virtual ~PanasonicInfraredConfig()
	{
	}
};

#endif
