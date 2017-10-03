#ifndef __ELEGOO_DISTANCE_UNIT_H__
#define __ELEGOO_DISTANCE_UNIT_H__

#include <Arduino.h>
#include <Servo.h>
#include "ElegooCarConfig.h"
#include "ElegooInterruptibleUnit.h"
#include "ElegooMath.h"

class DistanceData
{
public:
	int direction = 0;
	int distance = 0;
};

class ElegooDistanceUnit: public ElegooInterruptibleUnit
{
private:
	ElegooCarConfig::DistanceUnitConfig & config;

	Servo servo;

	static const int MIN_SERVO_DELAY = 500; // TODO (LOW) can we further reduce the value for MIN_DELAY

	static const int MAX_SCANNED_DISTANCES = 5;

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

	static const int HALF_RIGHT = 45;
	static const int FRONT = 90;
	static const int HALF_LEFT = 135;

	boolean isFarRightDirection(const int direction)
	{
		return (direction < HALF_RIGHT);

	}

	boolean isFarLeftDirection(const int direction)
	{
		return (direction > HALF_LEFT);

	}

	ElegooDistanceUnit(ElegooCarConfig::DistanceUnitConfig & pConfig) :
			ElegooInterruptibleUnit(), //
			config(pConfig), //
			servo()
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
		// performs a "full" scan
		scanBiggestDistance();
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

	DistanceData scanBiggestDistance()
	{
		DistanceData distances[MAX_SCANNED_DISTANCES];
		scanDistances(distances);

		DistanceData biggestDistance = distances[0];

		for (int i = 1; i < MAX_SCANNED_DISTANCES; i++)
		{
			if (distances[i].distance > biggestDistance.distance)
			{
				biggestDistance = distances[i];
			}
		}

		return biggestDistance;
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
		debugOrgVsAdjDirection(direction, adjustedDirection);
#endif

		if (_direction != adjustedDirection)
		{
			int delayInMS = ElegooMath::distance(_direction, adjustedDirection); // values between 0-180
			delayInMS *= 10; // values between 0-1800
			if (delayInMS < MIN_SERVO_DELAY)
			{
				delayInMS = MIN_SERVO_DELAY;
			}
			delay(delayInMS);
		}
		_direction = adjustedDirection;
	}

#if DEBUG_THE_CAR
	void debugOrgVsAdjDirection(int direction, int adjustedDirection)
	{
		Serial.print("ORG direction: ");
		Serial.print(direction);
		Serial.print(" => ADJ direction: ");
		Serial.println(adjustedDirection);
	}
#endif

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

	void scanDistances(DistanceData distances[])
	{
		distances[0] = readDistanceDataForDirection(config.SERVO_RIGHT);
		distances[1] = readDistanceDataForDirection(HALF_RIGHT);
		distances[2] = readDistanceDataForDirection(FRONT);
		distances[3] = readDistanceDataForDirection(HALF_LEFT);
		distances[4] = readDistanceDataForDirection(config.SERVO_LEFT);
	}

	DistanceData readDistanceDataForDirection(const int direction)
	{
		int distance = readDistanceForDirection(direction);
		DistanceData distanceData;
		distanceData.direction = direction;
		distanceData.distance = distance;
		return distanceData;
	}

	int readDistanceForDirection(const int direction)
	{
		if (hasCommand())
		{
			return 0;
		}

		setDirection(direction);
		int distance = readDistance();
#if DEBUG_THE_CAR
		debugDistanceForDirection(direction, distance);
#endif
		return distance;
	}

#if DEBUG_THE_CAR
	void debugDistanceForDirection(const int direction, const int distance)
	{
		Serial.print("Direction ");
		Serial.print(getDirectionString(direction));
		Serial.print(" = ");
		Serial.println(distance);
	}
#endif

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
