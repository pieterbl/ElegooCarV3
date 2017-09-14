
//www.elegoo.com

//    Left motor truth table
//  ENA         IN1               IN2         Description
//  LOW   Not Applicable    Not Applicable    Motor is off
//  HIGH        LOW               LOW         Motor is stopped (brakes)
//  HIGH        HIGH              LOW         Motor is on and turning forwards
//  HIGH        LOW               HIGH        Motor is on and turning backwards
//  HIGH        HIGH              HIGH        Motor is stopped (brakes)

//    Right motor truth table
//  ENB         IN3             IN4         Description
//  LOW   Not Applicable   Not Applicable   Motor is off
//  HIGH        LOW             LOW         Motor is stopped (brakes)
//  HIGH        LOW             HIGH        Motor is on and turning forwards
//  HIGH        HIGH            LOW         Motor is on and turning backwards
//  HIGH        HIGH            HIGH        Motor is stopped (brakes)

//    The direction of the car's movement
//  Left motor    Right motor     Description
//  stop(off)     stop(off)       Car is stopped
//  forward       forward         Car is running forwards
//  forward       backward        Car is turning right
//  backward      forward         Car is turning left
//  backward      backward        Car is running backwards

//define the L298n IO pin
#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11
#include <Arduino.h>

void setup() {
	Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
}
void loop() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);  //go forward
  delay(500);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);   //stop
  delay(500);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);   //go back
  delay(500);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, HIGH);  //stop
  delay(500);
}


/*
#include "Arduino.h"
#include "ElegooInfraredConfig.h"

class PanasonicInfraredConfig: public ElegooInfraredConfigInterface // TODO move to own header file
{
public:
	virtual ElegooMoveCommand checkCommand(unsigned long possibleCommandCode)
	{
		switch (possibleCommandCode)
		{
		case 3810251948:
			return ElegooMoveCommand::MOVE_FORWARDS;
		case 4025005874:
			return ElegooMoveCommand::TURN_RIGHT;
		case 3039307748:
			return ElegooMoveCommand::TURN_LEFT;
		case 2830352306:
			return ElegooMoveCommand::MOVE_BACKWARDS;
		case 3416630992:
			return ElegooMoveCommand::STOP_MOVING;
		default:
			return ElegooMoveCommand::UNKNOWN_CMD;
		}
	}

	virtual ~PanasonicInfraredConfig()
	{
	}
};

#include "ElegooCarV3.h"
#include "MyBluetoothConfig.h"

ElegooCarV3 * car = NULL;

void setup()
{
	ElegooCarConfig * carConfig = new ElegooCarConfig();
	carConfig->SAFETY_DISTANCE_CM = 25;
	carConfig->serialConfig.BAUD_RATE = 9600;
	carConfig->infraredReceiverConfig.MAX_NUM_RECEIVERS = 4;
	carConfig->bluetoothReceiverConfig.MAX_NUM_RECEIVERS = 4;

	car = new ElegooCarV3(carConfig);
	car->setup();
	car->registerInfraredConfig(new ElegooInfraredConfig());
	car->registerInfraredConfig(new PanasonicInfraredConfig());
	car->registerBluetoothConfig(new MyBluetoothConfig());
}

void loop()
{
	car->drive();
}

*/
