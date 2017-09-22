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

	// positively tested that we detect NO_COMMAND, as long as no buttons are pressed
	void testInfrared()
	{
		long noCmdCnt = 0;
		while (true)
		{
			ElegooCommand cmd = infraredReceiver.readCommand();
			if (cmd == ElegooCommand::NO_COMMAND)
			{
				noCmdCnt++;
			}
			else
			{
				const char * cmdString = ElegooCommandUtil::getCommandString(cmd);
				Serial.print(noCmdCnt);
				Serial.print(": ");
				Serial.println(cmdString);

				if (cmd == ElegooCommand::STOP_MOVING)
				{
					return;
				}
			}
		}
	}

	// positively tested that we detect NO_COMMAND, as long as no buttons are pressed
	void testBluetooth()
	{
		long noCmdCnt = 0;
		while (true)
		{
			ElegooCommand cmd = bluetoothReceiver.readCommand();
			if (cmd == ElegooCommand::NO_COMMAND)
			{
				noCmdCnt++;
			}
			else
			{
				const char * cmdString = ElegooCommandUtil::getCommandString(cmd);
				Serial.print(noCmdCnt);
				Serial.print(": ");
				Serial.println(cmdString);

				if (cmd == ElegooCommand::STOP_MOVING)
				{
					return;
				}
			}
		}
	}
};

#endif
