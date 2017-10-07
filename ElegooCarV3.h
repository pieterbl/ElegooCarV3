#ifndef __ELEGOO_CAR_V3_H__
#define __ELEGOO_CAR_V3_H__

#include <Arduino.h>
#include "ElegooConstants.h"
#include "ElegooCarConfig.h"
#include "ElegooDistanceUnit.h"
#include "ElegooMotorUnit.h"
#include "ElegooInfraredReceiver.h"
#include "ElegooBluetoothReceiver.h"
#include "ElegooCommandReader.h"
#include "ElegooManualDriver.h"
#include "ElegooManualDriver2.h"
#include "ElegooAutomaticDriver1.h"
#include "ElegooAutomaticDriver2.h"
#include "ElegooLineTrackingDriver.h"

class ElegooCarV3
{
private:

	ElegooCarConfig * carConfig;

	ElegooDistanceUnit distUnit;

	ElegooMotorUnit motorUnit;

	ElegooInfraredReceiver infraredReceiver;

	ElegooBluetoothReceiver bluetoothReceiver;

	ElegooCommandReader commandReader;

	ElegooDriverBase * drivers[5];

	ElegooDriverBase * currentDriver = 0;

	int safetyDistanceInCM;

public:

	ElegooCarV3(ElegooCarConfig * pCarConfig) :
			carConfig(pCarConfig), //
			distUnit(carConfig->distanceUnitConfig), //
			motorUnit(carConfig->motorUnitConfig), //
			infraredReceiver(carConfig->infraredReceiverConfig), //
			bluetoothReceiver(carConfig->bluetoothReceiverConfig), //
			commandReader(infraredReceiver, bluetoothReceiver), //
			safetyDistanceInCM(carConfig->SAFETY_DISTANCE_CM)
	{
	}

	int setup()
	{
		Serial.begin(carConfig->serialConfig.BAUD_RATE);
		distUnit.setup();
		distUnit.registerCommandReader(&commandReader);
		motorUnit.setup();
		motorUnit.registerCommandReader(&commandReader);
		infraredReceiver.setup();
		bluetoothReceiver.setup();

		initializeDrivers();
		selectManualDriver();

		return ElegooConstants::OK;
	}

private:
	void initializeDrivers()
	{
		drivers[ElegooCommand::MANUAL_DRIVER_1] = //
				new ElegooManualDriver(safetyDistanceInCM, distUnit, motorUnit);

		drivers[ElegooCommand::MANUAL_DRIVER_2] = //
				new ElegooManualDriver2(safetyDistanceInCM, distUnit, motorUnit);

		drivers[ElegooCommand::AUTO_DRIVER_1] = //
				new ElegooAutomaticDriver1(safetyDistanceInCM, distUnit, motorUnit);

		drivers[ElegooCommand::AUTO_DRIVER_2] = //
				new ElegooAutomaticDriver2(safetyDistanceInCM, distUnit, motorUnit);

		drivers[ElegooCommand::LINE_TRACKING_DRIVER] = //
				new ElegooLineTrackingDriver(safetyDistanceInCM, distUnit, motorUnit);
	}

	bool isDriver(ElegooCommand newDriver)
	{
		return (newDriver == ElegooCommand::MANUAL_DRIVER_1 || //
				newDriver == ElegooCommand::MANUAL_DRIVER_2 || //
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
		return (currentDriver == drivers[ElegooCommand::MANUAL_DRIVER_1] || //
				currentDriver == drivers[ElegooCommand::MANUAL_DRIVER_2]);
	}

public:
	int selectManualDriver()
	{
		return selectDriver(ElegooCommand::MANUAL_DRIVER_1);
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
		ElegooCommand cmd = commandReader.readCommand();
		if (cmd == ElegooCommand::STOP_MOVING)
		{
			motorUnit.stopMoving();
			selectManualDriver();
			return ElegooConstants::OK;
		}

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
			case ElegooCommand::MANUAL_DRIVER_1:
			case ElegooCommand::MANUAL_DRIVER_2:
			case ElegooCommand::AUTO_DRIVER_1:
			case ElegooCommand::AUTO_DRIVER_2:
			case ElegooCommand::LINE_TRACKING_DRIVER:
				return selectDriver(cmd);

			default:
				// manual drivers do correctly handle UNK_COMMAND and even rely on NO_COMMAND
				return currentDriver->processCommand(cmd);
			}
		}

		// automatic drivers will/must not listen to commands
		// they must just get "re-triggered"
		return currentDriver->processCommand(ElegooCommand::NO_COMMAND);
	}

	void testDistanceUnit()
	{
		Serial.println("Test Distance Unit");
		distUnit.frontDistance();
		distUnit.test();
		distUnit.frontDistance();
		Serial.println();
	}

	void testInfrared()
	{
		commandReader.testInfrared();
	}

	void testBluetooth()
	{
		commandReader.testBluetooth();
	}

};

#endif
