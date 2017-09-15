#ifndef __ELEGOO_MOVE_COMMAND_H__
#define __ELEGOO_MOVE_COMMAND_H__

enum ElegooMoveCommand
{
	UNKNOWN_CMD, MOVE_FORWARDS, TURN_RIGHT, HALF_RIGHT, TURN_LEFT, HALF_LEFT, MOVE_BACKWARDS, STOP_MOVING
};

class ElegooMoveCommandUtil
{
public:
	const char* getMoveCommandString(ElegooMoveCommand moveCommand)
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
		default:
			return "Unknown Command";
		}
	}

};

#endif
