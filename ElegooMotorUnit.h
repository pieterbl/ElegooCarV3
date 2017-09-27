#ifndef __ELEGOO_MOTOR_UNIT_H__
#define __ELEGOO_MOTOR_UNIT_H__

#include <Arduino.h>
#include "ElegooBase.h"
#include "ElegooCommand.h"
#include "ElegooCommandReader.h"
#include "ElegooCarConfig.h"

#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11

class Action
{
public:
	Action()
	{
	}

	virtual void execute()
	{
	}

	virtual ~Action()
	{
	}
};

class DriveAction: public Action
{
private:
	uint8_t pin1, pin2, pin3, pin4;
	uint8_t val1, val2, val3, val4;

public:
	DriveAction( //
			uint8_t pPin1, uint8_t pVal1, //
			uint8_t pPin2, uint8_t pVal2, //
			uint8_t pPin3, uint8_t pVal3, //
			uint8_t pPin4, uint8_t pVal4) : //
			Action()
	{
		pin1 = pPin1, pin2 = pPin2, pin3 = pPin3, pin4 = pPin4;
		val1 = pVal1, val2 = pVal2, val3 = pVal3, val4 = pVal4;
	}

	virtual void execute()
	{
		digitalWrite(pin1, val1);
		digitalWrite(pin2, val2);
		digitalWrite(pin3, val3);
		digitalWrite(pin4, val4);
	}

	virtual ~DriveAction()
	{
	}
};

class ElegooMotorUnit: public ElegooBase
{
private:

	ElegooCarConfig::MotorUnitConfig & config;

	ElegooCommandReader * commandReader;

public:

	ElegooMotorUnit(ElegooCarConfig::MotorUnitConfig & pMotorUnitConfig) :
			ElegooBase(), //
			config(pMotorUnitConfig), //
			commandReader(0)
	{
	}

	ElegooMotorUnit & setup()
	{
		pinMode(ENA, OUTPUT);
		pinMode(ENB, OUTPUT);
		pinMode(IN1, OUTPUT);
		pinMode(IN2, OUTPUT);
		pinMode(IN3, OUTPUT);
		pinMode(IN4, OUTPUT);
		return *this;
	}

	ElegooMotorUnit & registerCommandReader(ElegooCommandReader * pCommandReader)
	{
		commandReader = pCommandReader;
		return *this;
	}

	ElegooMotorUnit & moveForwards(int delayMS = 0)
	{
		printMovement(delayMS, ElegooCommand::MOVE_FORWARDS);
		moveWheelsForTime(HIGH, LOW, LOW, HIGH, delayMS);
		return *this;
	}

	ElegooMotorUnit & moveBackwards(int delayMS = 500)
	{
		printMovement(delayMS, ElegooCommand::MOVE_BACKWARDS);
		moveWheelsForTime(LOW, HIGH, HIGH, LOW, delayMS);
		return *this;
	}

	ElegooMotorUnit & turnLeft(int delayMS = 500)
	{
		printMovement(delayMS, ElegooCommand::TURN_LEFT);
		moveWheelsForTime(LOW, HIGH, LOW, HIGH, delayMS);
		return *this;
	}

	ElegooMotorUnit & turnHalfLeft(int delayMS = 250)
	{
		return turnLeft(delayMS);
	}

	ElegooMotorUnit & turnRight(int delayMS = 500)
	{
		printMovement(delayMS, ElegooCommand::TURN_RIGHT);
		moveWheelsForTime(HIGH, LOW, HIGH, LOW, delayMS);
		return *this;
	}

	ElegooMotorUnit & turnHalfRight(int delayMS = 250)
	{
		return turnRight(delayMS);
	}

	ElegooMotorUnit & stopMoving(int delayMS = 250)
	{
		printMovement(delayMS, ElegooCommand::STOP_MOVING);

		stopWheels();

		Action noAction;

		return runInterruptableAction(noAction, delayMS);
	}

private:

	bool hasCommand()
	{
		if (commandReader == 0)
		{
			return false;
		}

		return commandReader->hasCommand();
	}

	ElegooMotorUnit & powerOnWheels()
	{
		analogWrite(ENA, config.SPEED);
		analogWrite(ENB, config.SPEED);
		return *this;
	}

	ElegooMotorUnit & stopWheels()
	{
		digitalWrite(ENA, LOW);
		digitalWrite(ENB, LOW);
		return *this;
	}

	ElegooMotorUnit & moveWheelsForTime(uint8_t valIn1, uint8_t valIn2, uint8_t valIn3, uint8_t valIn4, int timeMS)
	{
		powerOnWheels();

		DriveAction driveAction(IN1, valIn1, IN2, valIn2, IN3, valIn3, IN4, valIn4);

		return runInterruptableAction(driveAction, timeMS);
	}

	ElegooMotorUnit & runInterruptableAction(Action & action, int delayTotalMS, int delayIncrementMS = 50)
	{
		// call execute, so that in case delayTotalMS==0, we at least execute once
		action.execute();

		for (int i = 0; i < delayTotalMS; i += delayIncrementMS)
		{
			if (hasCommand())
			{
				return *this;
			}

			delay(delayIncrementMS);

			if (hasCommand())
			{
				return *this;
			}

			action.execute();
		}

		return *this;
	}

	ElegooMotorUnit & printMovement(int delayMs, ElegooCommand cmd)
	{
		Serial.print(ElegooCommandUtil::getCommandString(cmd));
		Serial.print(": ");
		Serial.print(delayMs);
		Serial.println(" MS");
		return *this;
	}
};

#endif

