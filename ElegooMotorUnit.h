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

class ElegooMotorUnit: public ElegooBase
{
private:

	ElegooCarConfig::MotorUnitConfig & config;

	ElegooCommandReader * commandReader;

public:

	ElegooMotorUnit(ElegooCarConfig::MotorUnitConfig & pMotorUnitConfig) :
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
		delay(delayMS);
		return *this;
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

		for (int i = 0; i < timeMS; i += 50)
		{
			digitalWrite(IN1, valIn1);
			digitalWrite(IN2, valIn2);
			digitalWrite(IN3, valIn3);
			digitalWrite(IN4, valIn4);

			// have this check behind the digitalWrite statements, so that in case timeMS==0, we also move the wheels
			if (hasCommand())
			{
				return *this;
			}
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

