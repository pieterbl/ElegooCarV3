#ifndef __ELEGOO_MOVE_COMMAND_H__
#define __ELEGOO_MOVE_COMMAND_H__

enum ElegooMoveCommand
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
	UNKNOWN_CMD, // TODO rename into UNK_COMMAND
	NO_COMMAND
};

class ElegooMoveCommandUtil
{
public:
	static bool isValidCommand(ElegooMoveCommand moveCommand)
	{
		return (moveCommand != ElegooMoveCommand::UNKNOWN_CMD) && (moveCommand != ElegooMoveCommand::NO_COMMAND);
	}

	static const char* getMoveCommandString(ElegooMoveCommand moveCommand)
	{
		switch (moveCommand)
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
		case UNKNOWN_CMD:
			return "Unknown Command";
		case NO_COMMAND:
			return "No Command";
		default:
			return "ERROR";
		}
	}

};

#endif
