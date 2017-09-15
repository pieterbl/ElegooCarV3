#ifndef __ELEGOO_DISTANCE_UNIT_H__
#define __ELEGOO_DISTANCE_UNIT_H__

#include <Arduino.h>
#include <Servo.h>
#include "ElegooCarConfig.h"

class ElegooDistanceUnit
{
private:
	ElegooCarConfig::DistanceUnitConfig & config;

	Servo servo;

	static const int HALF_RIGHT = 45;
	static const int FRONT = 90;
	static const int HALF_LEFT = 135;

	const char* getDirectionString(const int direction)
	{
		if (direction == config.SERVO_RIGHT)
		{
			return "Right";
		}

		if (direction == config.SERVO_LEFT)
		{
			return "Left";
		}

		switch (direction)
		{
		case HALF_RIGHT:
			return "Half Right";
		case FRONT:
			return "Front";
		case HALF_LEFT:
			return "Half Left";
		default:
			return "Unknown";
		}
	}

public:

	ElegooDistanceUnit(ElegooCarConfig::DistanceUnitConfig & pConfig) :
			config(pConfig), servo()
	{
	}

	void setup()
	{
		servo.attach(config.SERVO_PIN);
		pinMode(config.ECHO_PIN, INPUT);
		pinMode(config.TRIGGER_PIN, INPUT);
	}

	void test()
	{
		rightDistance();
		readDistanceForDirection(HALF_RIGHT);
		frontDistance();
		readDistanceForDirection(HALF_LEFT);
		leftDistance();
	}

	int frontDistance()
	{
		return readDistanceForDirection(FRONT);
	}

	int rightDistance()
	{
		return readDistanceForDirection(config.SERVO_RIGHT);
	}

	int leftDistance()
	{
		return readDistanceForDirection(config.SERVO_LEFT);
	}

private:

	int _direction = -1;

	void setDirection(const int direction)
	{
		// direction parameter will be checked against what is defined as min/max in the config object
		int minPos = getServoMinPos();
		int maxPos = getServoMaxPos();

		int adjustedDirection = direction;
		if (direction < minPos)
		{
			adjustedDirection = minPos;
		}
		if (direction > maxPos)
		{
			adjustedDirection = maxPos;
		}
		servo.write(adjustedDirection);

#if DEBUG_THE_CAR
		Serial.print("ORG direction: ");
		Serial.print(direction);
		Serial.print(" => ADJ direction: ");
		Serial.println(adjustedDirection);
#endif

		if (_direction != adjustedDirection)
		{
			const int delayInMS = 1500; // TODO optimize? if change in direction is small, 500 ms will also do
			delay(delayInMS);
		}
		_direction = adjustedDirection;
	}

	// ultrasonic distance measurement function
	int readDistance()
	{
		digitalWrite(config.TRIGGER_PIN, LOW);
		delayMicroseconds(2);
		digitalWrite(config.TRIGGER_PIN, HIGH);
		delayMicroseconds(20);
		digitalWrite(config.TRIGGER_PIN, LOW);

		float fDistance = pulseIn(config.ECHO_PIN, HIGH);
		fDistance = fDistance / 58;
		return (int) fDistance;
	}

	int readDistanceForDirection(const int direction)
	{
		setDirection(direction);
		int distance = readDistance();

		Serial.print("Direction ");
		Serial.print(getDirectionString(direction));
		Serial.print(" = ");
		Serial.println(distance);

		return distance;
	}

	int getServoMinPos()
	{
		return min(config.SERVO_RIGHT, config.SERVO_LEFT);
	}

	int getServoMaxPos()
	{
		return max(config.SERVO_RIGHT, config.SERVO_LEFT);
	}

};

#endif
