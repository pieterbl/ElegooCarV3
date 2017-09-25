#include <Arduino.h>

#define DEBUG_THE_CAR 1
#define TEST_THE_CAR 0

#include "ElegooCarV3.h"
#include "ElegooInfraredConfig.h"
#include "PanasonicInfraredConfig.h"
#include "MyBluetoothConfig.h"

ElegooCarV3 * car = 0;

void setup()
{
	ElegooCarConfig * carConfig = new ElegooCarConfig();
	carConfig->SAFETY_DISTANCE_CM = 30;
	carConfig->serialConfig.BAUD_RATE = 9600;
	carConfig->distanceUnitConfig.SERVO_RIGHT = 20;
	carConfig->distanceUnitConfig.SERVO_LEFT = 180;
	carConfig->motorUnitConfig.SPEED = 170;
	carConfig->infraredReceiverConfig.MAX_NUM_RECEIVERS = 4;
	carConfig->bluetoothReceiverConfig.MAX_NUM_RECEIVERS = 4;

	car = new ElegooCarV3(carConfig);
	car->setup();
	car->selectManualDriver();

	car->registerInfraredConfig(new ElegooInfraredConfig());
	car->registerInfraredConfig(new PanasonicInfraredConfig());
	car->registerBluetoothConfig(new MyBluetoothConfig());
	car->testDistanceUnit();

	if (TEST_THE_CAR)
	{
		car->testInfrared();
		car->testBluetooth();
	}
}

void loop()
{
	car->drive();
}
