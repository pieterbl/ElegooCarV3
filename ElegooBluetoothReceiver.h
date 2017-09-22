#ifndef __ELEGOO_BLUETOOTH_RECEIVER_H__
#define __ELEGOO_BLUETOOTH_RECEIVER_H__

#include <Arduino.h>

#include "ElegooBluetoothConfigInterface.h"
#include "ElegooCarConfig.h"
#include "ElegooCommand.h"
#include "ElegooReceiver.h"

class ElegooBluetoothReceiver: public ElegooReceiver
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
	ElegooCommand readCommand()
	{
		int intInput = Serial.read();
		if (intInput == -1)
		{
			return ElegooCommand::NO_COMMAND;
		}

		char charInput = (char) intInput;
		ElegooCommand cmd = checkBluetoothProviders(charInput);
		return cmd;
	}

private:

	// May return UNKNOWN_CMD, will never return NO_COMMAND
	ElegooCommand checkBluetoothProviders(char charInput)
	{
		for (int i = 0; i < numBluetoothConfigs; i++)
		{
			ElegooBluetoothConfigInterface * bluetoothConfig = bluetoothConfigs[i];
			if (bluetoothConfig->isSupportedCharacter(charInput))
			{
				Serial.print("Bluetooth result: ");
				Serial.println(charInput);
			}

			ElegooCommand cmd = bluetoothConfig->checkCommand(charInput);
			if (cmd != ElegooCommand::UNK_COMMAND)
			{
				return cmd; // return as soon as we have found something known
			}
		}

		return ElegooCommand::UNK_COMMAND;
	}
};

#endif

