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
#include "ElegooAutomaticDriver1.h"
#include "ElegooAutomaticDriver2.h"
#include "ElegooLineTrackingDriver.h"

/*
 * TODO (HIGH: TO-TEST) most of the driving functions cannot be interrupted by a button press or so, this must be fixed (need to check Arduino interrupts, too).
 * Affected classes:
 * - All driver classes
 * - Driver base class (backOut function)
 */
class ElegooCarV3
{
private:

	ElegooCarConfig * carConfig;

	ElegooDistanceUnit distUnit;

	ElegooMotorUnit motorUnit;

	ElegooInfraredReceiver infraredReceiver; // TODO (LOW) Move to inside the ElegooCommand Reader?

	ElegooBluetoothReceiver bluetoothReceiver; // TODO (LOW) Move to inside the ElegooCommand Reader?

	ElegooCommandReader commandReader;

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
