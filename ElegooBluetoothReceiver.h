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

	ElegooBluetoothReceiver(ElegooCarConfig::BluetoothReceiverConfig & pConfig): config(pConfig)
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

	ElegooMoveCommand readCommand()
	{
		const char charInput = Serial.read();
		ElegooMoveCommand moveCommand = checkBluetoothProviders(charInput);
		return moveCommand;
	}

private:
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
				return moveCommand;
			}
		}

		return ElegooMoveCommand::UNKNOWN_CMD;
	}
};

#endif

