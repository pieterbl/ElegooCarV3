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

	static const int RIGHT = 20; // TODO hard-coded (sometimes value 10 or 20 is better here)
	static const int HALF_RIGHT = 45;
	static const int FRONT = 90;
	static const int HALF_LEFT = 135;
	static const int LEFT = 180;

	const char* getDirectionString(const int direction)
	{
		switch (direction)
		{
		case RIGHT:
			return "Right";
		case HALF_RIGHT:
			return "Half-Right";
		case FRONT:
			return "Front";
		case HALF_LEFT:
			return "Half-Left";
		case LEFT:
			return "Left";
		default:
			return "Unknown/Error";
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

	int getServoMinPos()
	{
		return min(config.SERVO_RIGHT, config.SERVO_LEFT);
	}

	int getServoMaxPos()
	{
		return max(config.SERVO_RIGHT, config.SERVO_LEFT);
	}

	void test()
	{
		readDistanceForDirection(RIGHT);
		readDistanceForDirection(HALF_RIGHT);
		readDistanceForDirection(FRONT);
		readDistanceForDirection(HALF_LEFT);
		readDistanceForDirection(LEFT);
	}

	int frontDistance()
	{
		return readDistanceForDirection(FRONT);
	}

	int leftDistance()
	{
		return readDistanceForDirection(LEFT);
	}

	int rightDistance()
	{
		return readDistanceForDirection(RIGHT);
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

private:

	int _direction = -1;

	void setDirection(const int direction)
	{
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
};

#endif
