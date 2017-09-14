#ifndef __ELEGOO_CAR_V3_H__
#define __ELEGOO_CAR_V3_H__

#include <Arduino.h>
#include "ElegooCarConfig.h"
#include "ElegooDistanceUnit.h"
#include "ElegooInfraredReceiver.h"
#include "ElegooBluetoothReceiver.h"

class ElegooCarV3
{
private:

	static const int STATUS_OK = 0;

	ElegooCarConfig * carConfig;

	ElegooDistanceUnit distUnit;

	ElegooInfraredReceiver infraredReceiver;

	ElegooBluetoothReceiver bluetoothReceiver;

	int safetyDistanceInCM;

	void printLine(const char * message)
	{
		Serial.println(message);
	}

	int moveForwards()
	{
		printLine("Move Forwards");
		return STATUS_OK;
	}

	int moveBackwards()
	{
		printLine("Move Backwards");
		delay(500);
		return STATUS_OK;
	}

	int stopMoving()
	{
		printLine("Stop Moving");
		delay(500);
		return STATUS_OK;
	}

	int turnLeft()
	{
		printLine("Turn Left");
		delay(500);
		return STATUS_OK;
	}

	int turnRight()
	{
		printLine("Turn Right");
		delay(500);
		return STATUS_OK;
	}

public:

	ElegooCarV3(ElegooCarConfig * pCarConfig):
		carConfig(pCarConfig),
		infraredReceiver(carConfig->infraredReceiverConfig),
		bluetoothReceiver(carConfig->bluetoothReceiverConfig),
		safetyDistanceInCM(carConfig->SAFETY_DISTANCE_CM)
	{
	}

	int setup()
	{
		Serial.begin(carConfig->serialConfig.BAUD_RATE);
		infraredReceiver.setup();
		bluetoothReceiver.setup();
		distUnit.setup();
		distUnit.test();
		distUnit.test();
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
			return moveForwards();
		}

		// frontDistance <= safetyDistanceInCM !!
		stopMoving();

		const int rightDistance = distUnit.rightDistance();
		const int leftDistance = distUnit.leftDistance();

		if ((rightDistance > safetyDistanceInCM) && (rightDistance >= leftDistance))
		{
			return turnRight();
		}

		if ((leftDistance > safetyDistanceInCM) && leftDistance >= rightDistance)
		{
			return turnLeft();
		}

		return backOut();
	}

private:

	int backOut()
	{
		boolean doBackOut = true;
		do
		{
			moveBackwards();
			stopMoving();
			const int rightDistance = distUnit.rightDistance();
			const int leftDistance = distUnit.leftDistance();

			doBackOut = (rightDistance <= safetyDistanceInCM) && (leftDistance <= safetyDistanceInCM);

		} while (doBackOut);
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
			return this->moveForwards();
		case ElegooMoveCommand::TURN_RIGHT:
		case ElegooMoveCommand::HALF_RIGHT: // TODO HALF_RIGHT still to be supported
			return this->turnRight();
		case ElegooMoveCommand::TURN_LEFT:
		case ElegooMoveCommand::HALF_LEFT: // TODO HALF_LEFT still to be supported
			return this->turnLeft();
		case ElegooMoveCommand::MOVE_BACKWARDS: // TODO initiate manual override, we get here due to remote-control button-press
			return this->moveBackwards();
		case ElegooMoveCommand::STOP_MOVING: // TODO initiate manual override, we get here due to remote-control button-press
			return this->stopMoving();
		case ElegooMoveCommand::UNKNOWN_CMD:
			return STATUS_OK;
		}
		return STATUS_OK;
	}
};

#endif
