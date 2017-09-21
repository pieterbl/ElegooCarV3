#ifndef __ELEGOO_MOTOR_UNIT_H__
#define __ELEGOO_MOTOR_UNIT_H__

#include <Arduino.h>
#include "ElegooBase.h"
#include "ElegooCommand.h"
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

public:

	ElegooMotorUnit(ElegooCarConfig::MotorUnitConfig & pMotorUnitConfig) :
			config(pMotorUnitConfig)
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

	ElegooMotorUnit & moveForwards(int delayMS = 0)
	{
		printMovement(delayMS, ElegooCommand::MOVE_FORWARDS);
		moveWheels(HIGH, LOW, LOW, HIGH);
		delay(delayMS);
		return *this;
	}

	ElegooMotorUnit & moveBackwards(int delayMS = 500)
	{
		printMovement(delayMS, ElegooCommand::MOVE_BACKWARDS);
		moveWheels(LOW, HIGH, HIGH, LOW);
		delay(delayMS);
		return *this;
	}

	ElegooMotorUnit & turnLeft(int delayMS = 500)
	{
		printMovement(delayMS, ElegooCommand::TURN_LEFT);
		moveWheels(LOW, HIGH, LOW, HIGH);
		delay(delayMS);
		return *this;
	}

	ElegooMotorUnit & turnHalfLeft(int delayMS = 250)
	{
		return turnLeft(delayMS);
	}

	ElegooMotorUnit & turnRight(int delayMS = 500)
	{
		printMovement(delayMS, ElegooCommand::TURN_RIGHT);
		moveWheels(HIGH, LOW, HIGH, LOW);
		delay(delayMS);
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

	ElegooMotorUnit & moveWheels(uint8_t valIn1, uint8_t valIn2, uint8_t valIn3, uint8_t valIn4)
	{
		powerOnWheels();
		digitalWrite(IN1, valIn1);
		digitalWrite(IN2, valIn2);
		digitalWrite(IN3, valIn3);
		digitalWrite(IN4, valIn4);
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

