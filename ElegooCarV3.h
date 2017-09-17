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

	ElegooDriverBase * drivers[3];

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

		drivers[ElegooMoveCommand::MANUAL_DRIVER] = //
				new ElegooManualDriver(motorUnit);
		drivers[ElegooMoveCommand::AUTO_DRIVER_1] = //
				new ElegooAutomaticDriver1(safetyDistanceInCM, distUnit, motorUnit);
		drivers[ElegooMoveCommand::AUTO_DRIVER_2] = //
				new ElegooAutomaticDriver2(safetyDistanceInCM, distUnit, motorUnit);

		selectManualDriver();

		return ElegooConstants::OK;
	}

private:
	int selectDriver(ElegooMoveCommand newDriver)
	{
		if (newDriver == ElegooMoveCommand::MANUAL_DRIVER || //
				newDriver == ElegooMoveCommand::AUTO_DRIVER_1 || //
				newDriver == ElegooMoveCommand::AUTO_DRIVER_2)
		{
			currentDriver = drivers[newDriver];
		}
		return ElegooConstants::OK;
	}

	bool usingManualDriver()
	{
		return (currentDriver == drivers[ElegooMoveCommand::MANUAL_DRIVER]);
	}

public:
	void selectManualDriver()
	{
		selectDriver(ElegooMoveCommand::MANUAL_DRIVER);
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
		ElegooMoveCommand moveCmd = readMoveCommand();
		if (moveCmd == ElegooMoveCommand::STOP_MOVING)
		{
			return motorUnit.stopMoving();
		}

		// TODO test that we indeed get NO_COMMAND from both the Infrared and the Bluetooth remote controls
		if (!usingManualDriver() && moveCmd != ElegooMoveCommand::NO_COMMAND)
		{
			motorUnit.stopMoving();
			selectManualDriver();
			// continue processing the given command (button press) below
		}

		if (usingManualDriver())
		{
			switch (moveCmd)
			{
			case ElegooMoveCommand::MANUAL_DRIVER:
			case ElegooMoveCommand::AUTO_DRIVER_1:
			case ElegooMoveCommand::AUTO_DRIVER_2:
				return selectDriver(moveCmd);

			case ElegooMoveCommand::UNKNOWN_CMD:
			case ElegooMoveCommand::NO_COMMAND:
				return ElegooConstants::OK;

			default:
				break;
			}

			return currentDriver->processCommand(moveCmd);
		}
		else
		{
			return currentDriver->processCommand(ElegooMoveCommand::NO_COMMAND);
		}
	}

private:

	// May also return UNKNOWN_CMD or NO_COMMAND
	ElegooMoveCommand readMoveCommand()
	{
		ElegooMoveCommand cmd = ElegooMoveCommand::NO_COMMAND;

		cmd = infraredReceiver.readCommand();
		if (ElegooMoveCommandUtil::isValidCommand(cmd))
		{
			return cmd;
		}

		cmd = bluetoothReceiver.readCommand();
		if (ElegooMoveCommandUtil::isValidCommand(cmd))
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

	void testInfrared() // TODO make this work
	{
		while (true)
		{
			ElegooMoveCommand cmd = infraredReceiver.readCommand();
			const char * cmdString = ElegooMoveCommandUtil::getMoveCommandString(cmd);
			Serial.println(cmdString);
			if (cmd == ElegooMoveCommand::STOP_MOVING)
			{
				return;
			}
		}
	}

	void testBluetooth() // TODO make this work
	{
		while (true)
		{
			ElegooMoveCommand cmd = bluetoothReceiver.readCommand();
			const char * cmdString = ElegooMoveCommandUtil::getMoveCommandString(cmd);
			Serial.println(cmdString);
			if (cmd == ElegooMoveCommand::STOP_MOVING)
			{
				return;
			}
		}
	}

};

#endif
