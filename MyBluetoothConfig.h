#ifndef __MY_BLUETOOTH_CONFIG_H__
#define __MY_BLUETOOTH_CONFIG_H__

#include "ElegooBluetoothConfigInterface.h"

class MyBluetoothConfig: public ElegooBluetoothConfigInterface
{
public:
	virtual const char * getSupportedCharacters()
	{
		return "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-+/\\";
	}

	// May return UNKNOWN_CMD, will never return NO_COMMAND
	virtual ElegooMoveCommand checkCommand(const char possibleCommandCode)
	{
		switch (possibleCommandCode)
		{
		case 'F':
			return ElegooMoveCommand::MOVE_FORWARDS;
		case 'R':
			return ElegooMoveCommand::TURN_RIGHT;
		case '/':
			return ElegooMoveCommand::HALF_RIGHT;
		case 'L':
			return ElegooMoveCommand::TURN_LEFT;
		case '\\':
			return ElegooMoveCommand::HALF_LEFT;
		case 'B':
			return ElegooMoveCommand::MOVE_BACKWARDS;
		case 'S':
			return ElegooMoveCommand::STOP_MOVING;
		case 'M':
			return ElegooMoveCommand::MANUAL_DRIVER;
		case '1':
			return ElegooMoveCommand::AUTO_DRIVER_1;
		case '2':
			return ElegooMoveCommand::AUTO_DRIVER_2;
		default:
			return ElegooMoveCommand::UNKNOWN_CMD;
		}
	}

	virtual ~MyBluetoothConfig()
	{
	}
};

#endif
