#ifndef __ELEGOO_COMMAND_READER_H__
#define __ELEGOO_COMMAND_READER_H__

#include "ElegooCommand.h"
#include "ElegooInfraredReceiver.h"
#include "ElegooBluetoothReceiver.h"

class ElegooCommandReader
{
private:
	ElegooInfraredReceiver & infraredReceiver;

	ElegooBluetoothReceiver & bluetoothReceiver;

public:
	// TODO (LOW) If more receivers must be supported, we cannot stick to constructor arguments
	ElegooCommandReader(ElegooInfraredReceiver & pInfraredReceiver, ElegooBluetoothReceiver & pBluetoothReceiver) :
			infraredReceiver(pInfraredReceiver), bluetoothReceiver(pBluetoothReceiver)
	{
	}

	// May also return UNKNOWN_CMD or NO_COMMAND
	ElegooCommand readCommand()
	{
		ElegooCommand cmd = ElegooCommand::NO_COMMAND;

		cmd = infraredReceiver.readCommand();
		if (ElegooCommandUtil::isValidCommand(cmd))
		{
			return cmd;
		}

		cmd = bluetoothReceiver.readCommand();
		if (ElegooCommandUtil::isValidCommand(cmd))
		{
			return cmd;
		}

		return cmd;
	}

	void testInfrared() // TODO (LOW) make this work
	{
		while (true)
		{
			ElegooCommand cmd = infraredReceiver.readCommand();
			const char * cmdString = ElegooCommandUtil::getCommandString(cmd);
			Serial.println(cmdString);
			if (cmd == ElegooCommand::STOP_MOVING)
			{
				return;
			}
		}
	}

	void testBluetooth() // TODO (LOW) make this work
	{
		while (true)
		{
			ElegooCommand cmd = bluetoothReceiver.readCommand();
			const char * cmdString = ElegooCommandUtil::getCommandString(cmd);
			Serial.println(cmdString);
			if (cmd == ElegooCommand::STOP_MOVING)
			{
				return;
			}
		}
	}
};

#endif
