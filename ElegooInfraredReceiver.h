#ifndef __ELEGOO_INFRARED_RECEIVER_H__
#define __ELEGOO_INFRARED_RECEIVER_H__

#include <Arduino.h>
#include <IRremote.h>

#include "ElegooMoveCommand.h"
#include "ElegooInfraredConfigInterface.h"

class ElegooInfraredReceiver
{
private:

	IRrecv * irrecv = NULL;

	ElegooCarConfig::InfraredReceiverConfig & config;

	ElegooInfraredConfigInterface ** infraredConfigs;

	int numInfraredConfigs = 0;

public:

	ElegooInfraredReceiver(ElegooCarConfig::InfraredReceiverConfig & pConfig): config(pConfig)
	{
		int size = config.MAX_NUM_RECEIVERS;
		infraredConfigs = new ElegooInfraredConfigInterface*[size];
	}

	void setup()
	{
		pinMode(config.RECEIVER_PIN, INPUT);
		irrecv = new IRrecv(config.RECEIVER_PIN);
		irrecv->enableIRIn();
	}

	void registerInfraredConfig(ElegooInfraredConfigInterface * infraredConfig)
	{
		infraredConfigs[numInfraredConfigs] = infraredConfig;
		numInfraredConfigs++;
	}

	ElegooMoveCommand readCommand()
	{
		decode_results results;

		if (!irrecv->decode(&results))
		{
			return ElegooMoveCommand::UNKNOWN_CMD;
		}

		unsigned long resultsValue = results.value;
		irrecv->resume();
		delay(150);

		Serial.print("Infrared result: ");
		Serial.println(resultsValue);

		ElegooMoveCommand moveCommand = checkInfraredProviders(resultsValue);
		return moveCommand;
	}

private:

	ElegooMoveCommand checkInfraredProviders(unsigned long resultsValue)
	{
		for (int i = 0; i < numInfraredConfigs; i++)
		{
			ElegooInfraredConfigInterface * infraredConfig = infraredConfigs[i];
			ElegooMoveCommand moveCommand = infraredConfig->checkCommand(resultsValue);
			if (moveCommand != ElegooMoveCommand::UNKNOWN_CMD)
			{
				return moveCommand;
			}
		}

		return ElegooMoveCommand::UNKNOWN_CMD;
	}
};

#endif

