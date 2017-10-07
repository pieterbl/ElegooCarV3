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
	virtual ElegooCommand checkCommand(const char possibleCommandCode)
	{
		switch (possibleCommandCode)
		{
		case 'F':
			return ElegooCommand::MOVE_FORWARDS;
		case 'R':
			return ElegooCommand::TURN_RIGHT;
		case '/':
			return ElegooCommand::HALF_RIGHT;
		case 'L':
			return ElegooCommand::TURN_LEFT;
		case '\\':
			return ElegooCommand::HALF_LEFT;
		case 'B':
			return ElegooCommand::MOVE_BACKWARDS;
		case 'S':
			return ElegooCommand::STOP_MOVING;
		case 'M':
			return ElegooCommand::MANUAL_DRIVER_1;
		case 'N':
			return ElegooCommand::MANUAL_DRIVER_2;
		case '1':
			return ElegooCommand::AUTO_DRIVER_1;
		case '2':
			return ElegooCommand::AUTO_DRIVER_2;
		case 'T':
			return ElegooCommand::LINE_TRACKING_DRIVER;
		default:
			return ElegooCommand::UNK_COMMAND;
		}
	}

	virtual ~MyBluetoothConfig()
	{
	}
};

#endif
