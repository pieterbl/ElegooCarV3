#ifndef __ELEGOO_COMMAND_H__
#define __ELEGOO_COMMAND_H__

enum ElegooCommand // TODO Add line-tracking driver option
{
	MANUAL_DRIVER = 0, //
	AUTO_DRIVER_1 = 1,
	AUTO_DRIVER_2 = 2,
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
