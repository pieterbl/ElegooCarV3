#ifndef __ELEGOO_CAR_V3_H__
#define __ELEGOO_CAR_V3_H__

#include <Arduino.h>
#include "ElegooConstants.h"
#include "ElegooCarConfig.h"
#include "ElegooDistanceUnit.h"
#include "ElegooMotorUnit.h"
#include "ElegooInfraredReceiver.h"
#include "ElegooBluetoothReceiver.h"

class ElegooCarV3
{
private:

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
		return ElegooConstants::OK;
	}

	void registerInfraredConfig(ElegooInfraredConfigInterface * infraredConfig)
	{
		infraredReceiver.registerInfraredConfig(infraredConfig);
	}

	void registerBluetoothConfig(ElegooBluetoothConfigInterface * bluetoothConfig)
	{
		bluetoothReceiver.registerBluetoothConfig(bluetoothConfig);
	}

	bool manualMode = false;

	int drive()
	{
		ElegooMoveCommand moveCmd = readMoveCommand();
		if (moveCmd != ElegooMoveCommand::UNKNOWN_CMD)
		{
			manualMode = true;
			processMoveCommand(moveCmd);
		}

		if (manualMode == true) // TODO test manual mode
		{
			return ElegooConstants::OK;
		}

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
			return ElegooConstants::OK;
		}

		if ((leftDistance > safetyDistanceInCM) && leftDistance >= rightDistance)
		{
			motorUnit.turnLeft();
			motorUnit.stopMoving();
			return ElegooConstants::OK;
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
		return ElegooConstants::OK;
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
			motorUnit.moveForwards();
			delay(1000);
			motorUnit.stopMoving();
			return ElegooConstants::OK;

		case ElegooMoveCommand::MOVE_BACKWARDS:
			motorUnit.moveBackwards();
			motorUnit.stopMoving();
			return ElegooConstants::OK;

		case ElegooMoveCommand::HALF_RIGHT:
			motorUnit.turnRight(250); // MS
			motorUnit.stopMoving();
			return ElegooConstants::OK;

		case ElegooMoveCommand::TURN_RIGHT:
			motorUnit.turnRight(500); // MS
			motorUnit.stopMoving();
			return ElegooConstants::OK;

		case ElegooMoveCommand::HALF_LEFT:
			motorUnit.turnLeft(250); // MS
			motorUnit.stopMoving();
			return ElegooConstants::OK;

		case ElegooMoveCommand::TURN_LEFT:
			motorUnit.turnLeft(500); // MS
			motorUnit.stopMoving();
			return ElegooConstants::OK;

		case ElegooMoveCommand::STOP_MOVING:
			motorUnit.stopMoving();
			return ElegooConstants::OK;

		case ElegooMoveCommand::UNKNOWN_CMD:
			return ElegooConstants::OK;
		}

		return ElegooConstants::OK;
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

};

#endif
