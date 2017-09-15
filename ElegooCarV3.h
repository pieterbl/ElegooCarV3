#ifndef __ELEGOO_CAR_V3_H__
#define __ELEGOO_CAR_V3_H__

#include <Arduino.h>
#include "ElegooCarConfig.h"
#include "ElegooDistanceUnit.h"
#include "ElegooMotorUnit.h"
#include "ElegooInfraredReceiver.h"
#include "ElegooBluetoothReceiver.h"

class ElegooCarV3
{
private:

	static const int STATUS_OK = 0;

	ElegooCarConfig * carConfig;

	ElegooDistanceUnit distUnit;

	ElegooMotorUnit motorUnit;

	ElegooInfraredReceiver infraredReceiver;

	ElegooBluetoothReceiver bluetoothReceiver;

	int safetyDistanceInCM;

public:

	ElegooCarV3(ElegooCarConfig * pCarConfig) :
			carConfig(pCarConfig), //
			distUnit(carConfig->distanceUnitConfig), //
			motorUnit(), //
			infraredReceiver(carConfig->infraredReceiverConfig), //
			bluetoothReceiver(carConfig->bluetoothReceiverConfig), //
			safetyDistanceInCM(carConfig->SAFETY_DISTANCE_CM)
	{
	}

	int setup()
	{
		Serial.begin(carConfig->serialConfig.BAUD_RATE);
		infraredReceiver.setup();
		bluetoothReceiver.setup();
		distUnit.setup();
		return STATUS_OK;
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
		// TODO pressing forward or backward should set: manual override, manual direction, backward to stay backward until changed
		ElegooMoveCommand moveCmd = readMoveCommand();
		processMoveCommand(moveCmd);

		const int frontDistance = distUnit.frontDistance();
		if (frontDistance > safetyDistanceInCM)
		{
			return motorUnit.moveForwards();
		}

		// frontDistance <= safetyDistanceInCM !!
		motorUnit.stopMoving();

		const int rightDistance = distUnit.rightDistance();
		const int leftDistance = distUnit.leftDistance();
		distUnit.frontDistance(); // reposition sensor, to avoid delays

		if ((rightDistance > safetyDistanceInCM) && (rightDistance >= leftDistance))
		{
			motorUnit.turnRight();
			motorUnit.stopMoving();
			return STATUS_OK;
		}

		if ((leftDistance > safetyDistanceInCM) && leftDistance >= rightDistance)
		{
			motorUnit.turnLeft();
			motorUnit.stopMoving();
			return STATUS_OK;
		}

		// we're stuck
		return backOut();
	}

private:

	int backOut()
	{
		boolean doBackOut = true;
		do
		{
			motorUnit.moveBackwards();
			motorUnit.stopMoving();
			const int rightDistance = distUnit.rightDistance();
			const int leftDistance = distUnit.leftDistance();

			doBackOut = (rightDistance <= safetyDistanceInCM) && (leftDistance <= safetyDistanceInCM);

		} while (doBackOut); // TODO result of backOut should be used ( if room at left or right, we should turn that way )
		return STATUS_OK;
	}

	ElegooMoveCommand readMoveCommand()
	{
		ElegooMoveCommand cmd = NULL;

		cmd = infraredReceiver.readCommand();
		if (cmd != ElegooMoveCommand::UNKNOWN_CMD)
		{
			return cmd;
		}

		cmd = bluetoothReceiver.readCommand();
		if (cmd != ElegooMoveCommand::UNKNOWN_CMD)
		{
			return cmd;
		}

		return ElegooMoveCommand::UNKNOWN_CMD;
	}

	int processMoveCommand(const ElegooMoveCommand cmd)
	{
		switch (cmd)
		{
		case ElegooMoveCommand::MOVE_FORWARDS:
			return motorUnit.moveForwards();
		case ElegooMoveCommand::TURN_RIGHT:
		case ElegooMoveCommand::HALF_RIGHT: // TODO HALF_RIGHT still to be supported
			return motorUnit.turnRight();
		case ElegooMoveCommand::TURN_LEFT:
		case ElegooMoveCommand::HALF_LEFT: // TODO HALF_LEFT still to be supported
			return motorUnit.turnLeft();
		case ElegooMoveCommand::MOVE_BACKWARDS: // TODO initiate manual override, we get here due to remote-control button-press
			return motorUnit.moveBackwards();
		case ElegooMoveCommand::STOP_MOVING: // TODO initiate manual override, we get here due to remote-control button-press
			return motorUnit.stopMoving();
		case ElegooMoveCommand::UNKNOWN_CMD:
			return STATUS_OK;
		}
		return STATUS_OK;
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

	void testInfrared() // TODO test it
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

	void testBluetooth() // TODO test it
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

	void testMotorUnit()
	{
		motorUnit.moveForwards();
		motorUnit.moveBackwards();
		motorUnit.turnLeft();
		motorUnit.turnRight();
	}

};

#endif
