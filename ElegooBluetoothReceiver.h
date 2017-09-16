#ifndef __ELEGOO_BLUETOOTH_RECEIVER_H__
#define __ELEGOO_BLUETOOTH_RECEIVER_H__

#include <Arduino.h>
#include "ElegooMoveCommand.h"
#include "ElegooBluetoothConfigInterface.h"
#include "ElegooCarConfig.h"

class ElegooBluetoothReceiver
{
private:

	ElegooCarConfig::BluetoothReceiverConfig & config;

	ElegooBluetoothConfigInterface ** bluetoothConfigs;

	int numBluetoothConfigs = 0;

public:

	ElegooBluetoothReceiver(ElegooCarConfig::BluetoothReceiverConfig & pConfig) :
			config(pConfig)
	{
		int size = config.MAX_NUM_RECEIVERS;
		bluetoothConfigs = new ElegooBluetoothConfigInterface*[size];
	}

	void setup()
	{
	}

	void registerBluetoothConfig(ElegooBluetoothConfigInterface * bluetoothConfig)
	{
		bluetoothConfigs[numBluetoothConfigs] = bluetoothConfig;
		numBluetoothConfigs++;
	}

	// May also return UNKNOWN_CMD or NO_COMMAND
	ElegooMoveCommand readCommand()
	{
		int intInput = Serial.read();
		if (intInput == -1)
		{
			return ElegooMoveCommand::NO_COMMAND;
		}

		char charInput = (char) intInput;
		ElegooMoveCommand moveCommand = checkBluetoothProviders(charInput);
		return moveCommand;
	}

private:

	// May return UNKNOWN_CMD, will never return NO_COMMAND
	ElegooMoveCommand checkBluetoothProviders(char charInput)
	{
		for (int i = 0; i < numBluetoothConfigs; i++)
		{
			ElegooBluetoothConfigInterface * bluetoothConfig = bluetoothConfigs[i];
			if (bluetoothConfig->isSupportedCharacter(charInput))
			{
				Serial.print("Bluetooth result: ");
				Serial.println(charInput);
			}

			ElegooMoveCommand moveCommand = bluetoothConfig->checkCommand(charInput);
			if (moveCommand != ElegooMoveCommand::UNKNOWN_CMD)
			{
				return moveCommand; // return as soon as we have found something known
			}
		}

		return ElegooMoveCommand::UNKNOWN_CMD;
	}
};

#endif

