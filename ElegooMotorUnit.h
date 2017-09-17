#ifndef __ELEGOO_MOTOR_UNIT_H__
#define __ELEGOO_MOTOR_UNIT_H__

#include <Arduino.h>
#include "ElegooCommand.h"
#include "ElegooConstants.h"
#include "ElegooCarConfig.h"

#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11

class ElegooMotorUnit
{
private:

	ElegooCarConfig::MotorUnitConfig & config;

public:

	ElegooMotorUnit(ElegooCarConfig::MotorUnitConfig & pMotorUnitConfig) :
			config(pMotorUnitConfig)
	{
	}

	void setup()
	{
		pinMode(ENA, OUTPUT);
		pinMode(ENB, OUTPUT);
		pinMode(IN1, OUTPUT);
		pinMode(IN2, OUTPUT);
		pinMode(IN3, OUTPUT);
		pinMode(IN4, OUTPUT);
	}

	int moveForwards(int delayMS = 0)
	{
		moveWheels(HIGH, LOW, LOW, HIGH);
		Serial.println(ElegooCommandUtil::getCommandString(ElegooCommand::MOVE_FORWARDS));
		delay(delayMS);
		return ElegooConstants::OK;
	}

	int moveBackwards(int delayMS = 500)
	{
		moveWheels(LOW, HIGH, HIGH, LOW);
		Serial.println(ElegooCommandUtil::getCommandString(ElegooCommand::MOVE_BACKWARDS));
		delay(delayMS);
		return ElegooConstants::OK;
	}

	int turnLeft(int delayMS = 500)
	{
		moveWheels(LOW, HIGH, LOW, HIGH);
		Serial.println(ElegooCommandUtil::getCommandString(ElegooCommand::TURN_LEFT));
		delay(delayMS);
		return ElegooConstants::OK;
	}

	int turnRight(int delayMS = 500)
	{
		moveWheels(HIGH, LOW, HIGH, LOW);
		Serial.println(ElegooCommandUtil::getCommandString(ElegooCommand::TURN_RIGHT));
		delay(delayMS);
		return ElegooConstants::OK;
	}

	int stopMoving(int delayMS = 250)
	{
		stopWheels();
		Serial.println(ElegooCommandUtil::getCommandString(ElegooCommand::STOP_MOVING));
		delay(delayMS);
		return ElegooConstants::OK;
	}

private:

	void powerOnWheels()
	{
		analogWrite(ENA, config.SPEED);
		analogWrite(ENB, config.SPEED);
	}

	void stopWheels()
	{
		digitalWrite(ENA, LOW);
		digitalWrite(ENB, LOW);
	}

	void moveWheels(uint8_t valIn1, uint8_t valIn2, uint8_t valIn3, uint8_t valIn4)
	{
		powerOnWheels();
		digitalWrite(IN1, valIn1);
		digitalWrite(IN2, valIn2);
		digitalWrite(IN3, valIn3);
		digitalWrite(IN4, valIn4);
	}
};

#endif

