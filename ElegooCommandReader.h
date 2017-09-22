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

	void testInfrared()
	{
		// positively tested that we detect NO_COMMAND, as long as no buttons are pressed
		ElegooCommandReader::testReceiver(infraredReceiver);
	}

	void testBluetooth()
	{
		// positively tested that we detect NO_COMMAND, as long as no buttons are pressed
		ElegooCommandReader::testReceiver(bluetoothReceiver);
	}

private:
	static void testReceiver(ElegooReceiver & receiver)
	{
		long noCmdCnt = 0;
		while (true)
		{
			ElegooCommand cmd = receiver.readCommand();
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
