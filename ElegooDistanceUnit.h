#ifndef __ELEGOO_DISTANCE_UNIT_H__
#define __ELEGOO_DISTANCE_UNIT_H__

#include <Arduino.h>
#include <Servo.h>

class ElegooDistanceUnit
{
private:
	static const int ECHO = A4; // TODO hard-coded
	static const int TRIGGER = A5; // TODO hard-coded
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

	ElegooDistanceUnit()
	{
	}

	void setup()
	{
		servo.attach(3); // TODO hard-coded
		pinMode(ECHO, INPUT); // TODO hard-coded
		pinMode(TRIGGER, OUTPUT); // TODO hard-coded
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

	void setDirection(const int direction)
	{
		int adjustedDirection = direction;
		if (direction < 0) // TODO hard-coded (sometimes value 10 or 20 is better here)
		{
			adjustedDirection = 0; // TODO hard-coded (sometimes value 10 or 20 is better here)
		}
		if (direction > 180)
		{
			adjustedDirection = 180;
		}
		servo.write(adjustedDirection);

		const int delayInMS = 1500; // TODO optimize, if change in direction is small, 500 ms will also do
		delay(delayInMS);
	}

	// ultrasonic distance measurement function
	static int readDistance()
	{
		digitalWrite(TRIGGER, LOW);
		delayMicroseconds(2);
		digitalWrite(TRIGGER, HIGH);
		delayMicroseconds(20);
		digitalWrite(TRIGGER, LOW);

		float fDistance = pulseIn(ECHO, HIGH);
		fDistance = fDistance / 58;
		return (int) fDistance;
	}
};

#endif
