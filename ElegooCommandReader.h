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

	static const int EMPTY_CACHE = -1;

	int cachedCommand = EMPTY_CACHE;

public:
	// TODO (P2-LOW) If more receivers must be supported, we cannot stick to constructor arguments
	ElegooCommandReader(ElegooInfraredReceiver & pInfraredReceiver, ElegooBluetoothReceiver & pBluetoothReceiver) :
			infraredReceiver(pInfraredReceiver), bluetoothReceiver(pBluetoothReceiver)
	{
	}

	bool hasCommand()
	{
		if (cachedCommand == EMPTY_CACHE)
		{
			cachedCommand = readTheCommand();
		}

		if (cachedCommand == ElegooCommand::NO_COMMAND)
		{
			cachedCommand = EMPTY_CACHE;
			return false;
		}
		else
		{
			return true;
		}
	}

	// May also return UNKNOWN_CMD or NO_COMMAND
	ElegooCommand readCommand()
	{
		if (hasCommand())
		{
			ElegooCommand theCmd = (ElegooCommand) cachedCommand;
			cachedCommand = EMPTY_CACHE;
			return theCmd;
		}
		else
		{
			return ElegooCommand::NO_COMMAND;
		}
	}

private:
	// May also return UNKNOWN_CMD or NO_COMMAND
	ElegooCommand readTheCommand()
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

public:
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
