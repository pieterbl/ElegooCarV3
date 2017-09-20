#ifndef __ELEGOO_CAR_V3_H__
#define __ELEGOO_CAR_V3_H__

#include <Arduino.h>
#include "ElegooConstants.h"
#include "ElegooCarConfig.h"
#include "ElegooDistanceUnit.h"
#include "ElegooMotorUnit.h"
#include "ElegooInfraredReceiver.h"
#include "ElegooBluetoothReceiver.h"
#include "ElegooManualDriver.h"
#include "ElegooAutomaticDriver1.h"
#include "ElegooAutomaticDriver2.h"

class ElegooCarV3
{
private:

	ElegooCarConfig * carConfig;

	ElegooDistanceUnit distUnit;

	ElegooMotorUnit motorUnit;

	ElegooInfraredReceiver infraredReceiver;

	ElegooBluetoothReceiver bluetoothReceiver;

	ElegooDriverBase * drivers[4];

	ElegooDriverBase * currentDriver = 0;

	int safetyDistanceInCM;

public:

	ElegooCarV3(ElegooCarConfig * pCarConfig) :
			carConfig(pCarConfig), //
			distUnit(carConfig->distanceUnitConfig), //
			motorUnit(carConfig->motorUnitConfig), //
			infraredReceiver(carConfig->infraredReceiverConfig), //
			bluetoothReceiver(carConfig->bluetoothReceiverConfig), //
			safetyDistanceInCM(carConfig->SAFETY_DISTANCE_CM)
	{
	}

	int setup()
	{
		Serial.begin(carConfig->serialConfig.BAUD_RATE);
		distUnit.setup();
		motorUnit.setup();
		infraredReceiver.setup();
		bluetoothReceiver.setup();

		initializeDrivers();
		selectManualDriver();

		return ElegooConstants::OK;
	}

private:
	void initializeDrivers()
	{
		drivers[ElegooCommand::MANUAL_DRIVER] = //
				new ElegooManualDriver(safetyDistanceInCM, distUnit, motorUnit);

		drivers[ElegooCommand::AUTO_DRIVER_1] = //
				new ElegooAutomaticDriver1(safetyDistanceInCM, distUnit, motorUnit);

		drivers[ElegooCommand::AUTO_DRIVER_2] = //
				new ElegooAutomaticDriver2(safetyDistanceInCM, distUnit, motorUnit);

		drivers[ElegooCommand::LINE_TRACKING_DRIVER] = //
				new ElegooLineTrackingDriver(safetyDistanceInCM, distUnit, motorUnit);
	}

	bool isDriver(ElegooCommand newDriver)
	{
		return (newDriver == ElegooCommand::MANUAL_DRIVER || //
				newDriver == ElegooCommand::AUTO_DRIVER_1 || //
				newDriver == ElegooCommand::AUTO_DRIVER_2 || //
				newDriver == ElegooCommand::LINE_TRACKING_DRIVER);
	}

	int selectDriver(ElegooCommand newDriver)
	{
		if (isDriver(newDriver))
		{
			currentDriver = drivers[newDriver];
		}
		return ElegooConstants::OK;
	}

	bool usingManualDriver()
	{
		return (currentDriver == drivers[ElegooCommand::MANUAL_DRIVER]);
	}

public:
	int selectManualDriver()
	{
		return selectDriver(ElegooCommand::MANUAL_DRIVER);
	}

	void registerInfraredConfig(ElegooInfraredConfigInterface * infraredConfig)
	{
		infraredReceiver.registerInfraredConfig(infraredConfig);
	}

	void registerBluetoothConfig(ElegooBluetoothConfigInterface * bluetoothConfig)
	{
		bluetoothReceiver.registerBluetoothConfig(bluetoothConfig);
	}

	int drive()
	{
		ElegooCommand cmd = readCommand();
		if (cmd == ElegooCommand::STOP_MOVING)
		{
			motorUnit.stopMoving();
			selectManualDriver();
			return ElegooConstants::OK;
		}

		// TODO test that we indeed get NO_COMMAND from both the Infrared and the Bluetooth remote controls
		if (!usingManualDriver() && cmd != ElegooCommand::NO_COMMAND)
		{
			motorUnit.stopMoving();
			selectManualDriver();
			// continue processing the given command (button press) below
		}

		if (usingManualDriver())
		{
			switch (cmd)
			{
			case ElegooCommand::MANUAL_DRIVER:
			case ElegooCommand::AUTO_DRIVER_1:
			case ElegooCommand::AUTO_DRIVER_2:
			case ElegooCommand::LINE_TRACKING_DRIVER:
				return selectDriver(cmd);

			case ElegooCommand::UNK_COMMAND:
			case ElegooCommand::NO_COMMAND:
				return ElegooConstants::OK;

			default:
				return currentDriver->processCommand(cmd);
			}
		}
		else
		{
			return currentDriver->processCommand(ElegooCommand::NO_COMMAND);
		}
	}

private:

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

public:

	void testDistanceUnit()
	{
		Serial.println("Test Distance Unit");
		distUnit.frontDistance();
		distUnit.test();
		distUnit.frontDistance();
		Serial.println();
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
