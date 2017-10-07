#ifndef __ELEGOO_COMMAND_H__
#define __ELEGOO_COMMAND_H__

enum ElegooCommand
{
	MANUAL_DRIVER_1 = 0, //
	MANUAL_DRIVER_2 = 1,
	AUTO_DRIVER_1 = 2,
	AUTO_DRIVER_2 = 3,
	LINE_TRACKING_DRIVER,
	MOVE_FORWARDS,
	MOVE_BACKWARDS,
	STOP_MOVING,
	TURN_RIGHT,
	HALF_RIGHT,
	TURN_LEFT,
	HALF_LEFT,
	UNK_COMMAND,
	NO_COMMAND
};

class ElegooCommandUtil
{
public:
	static bool isValidCommand(ElegooCommand cmd)
	{
		return (cmd != ElegooCommand::UNK_COMMAND) && (cmd != ElegooCommand::NO_COMMAND);
	}

	static const char* getCommandString(ElegooCommand cmd)
	{
		switch (cmd)
		{
		case MANUAL_DRIVER_1:
			return "Select Manual Driver 1";
		case MANUAL_DRIVER_2:
			return "Select Manual Driver 2";
		case AUTO_DRIVER_1:
			return "Select Autonomous Driver 1";
		case AUTO_DRIVER_2:
			return "Select Autonomous Driver 2";
		case LINE_TRACKING_DRIVER:
			return "Select Line Tracking Driver";
		case MOVE_FORWARDS:
			return "Move Forwards";
		case TURN_RIGHT:
			return "Turn Right";
		case HALF_RIGHT:
			return "Go Half Right";
		case TURN_LEFT:
			return "Turn Left";
		case HALF_LEFT:
			return "Go Half Left";
		case MOVE_BACKWARDS:
			return "Move Backwards";
		case STOP_MOVING:
			return "Stop Moving";
		case UNK_COMMAND:
			return "Unknown Command";
		case NO_COMMAND:
			return "No Command";
		default:
			return "ERROR";
		}
	}

};

#endif
