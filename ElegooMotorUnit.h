#ifndef __ELEGOO_MOTOR_UNIT_H__
#define __ELEGOO_MOTOR_UNIT_H__

#include <Arduino.h>

#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11

class ElegooMotorUnit // TODO Test the driving part
{
private:

	static const int STATUS_OK = 0;

	int speed = 100;

	void writeSpeedToBoard()
	{
		analogWrite(ENA, speed);
		analogWrite(ENB, speed);
	}

	void printLine(const char * message)
	{
		Serial.println(message);
	}

public:

	void setSpeed(int pSpeed)
	{
		speed = pSpeed;
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

	int moveForwards()
	{
		writeSpeedToBoard();
		digitalWrite(IN1, HIGH);
		digitalWrite(IN2, LOW);
		digitalWrite(IN3, LOW);
		digitalWrite(IN4, HIGH);
		printLine("Move Forwards");
		delay(500);
		return STATUS_OK;
	}

	int moveBackwards()
	{
		writeSpeedToBoard();
		digitalWrite(IN1, LOW);
		digitalWrite(IN2, HIGH);
		digitalWrite(IN3, HIGH);
		digitalWrite(IN4, LOW);
		printLine("Move Backwards");
		delay(500);
		return STATUS_OK;
	}

	int turnLeft()
	{
//		writeSpeedToBoard();
//		digitalWrite(IN1, LOW);
//		digitalWrite(IN2, HIGH);
//		digitalWrite(IN3, LOW);
//		digitalWrite(IN4, HIGH);
		printLine("Turn Left");
		delay(500);
		return STATUS_OK;
	}

	int turnRight()
	{
//		writeSpeedToBoard();
//		digitalWrite(IN1, HIGH);
//		digitalWrite(IN2, LOW);
//		digitalWrite(IN3, HIGH);
//		digitalWrite(IN4, LOW);
		printLine("Turn Right");
		delay(500);
		return STATUS_OK;
	}

	int stopMoving()
	{
		digitalWrite(ENA, LOW);
		digitalWrite(ENB, LOW);
		printLine("Stop Moving");
		delay(500);
		return STATUS_OK;
	}

};

#endif

