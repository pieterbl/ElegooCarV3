#include <Arduino.h>

#include "ElegooCarV3.h"
#include "ElegooInfraredConfig.h"
#include "PanasonicInfraredConfig.h"
#include "MyBluetoothConfig.h"

ElegooCarV3 * car = NULL;

void setup() // TODO test the car
{
	ElegooCarConfig * carConfig = new ElegooCarConfig();
	carConfig->SAFETY_DISTANCE_CM = 30;
	carConfig->serialConfig.BAUD_RATE = 9600;
	carConfig->distanceUnitConfig.SERVO_RIGHT = 0;
	carConfig->distanceUnitConfig.SERVO_LEFT = 180;
	carConfig->infraredReceiverConfig.MAX_NUM_RECEIVERS = 4;
	carConfig->bluetoothReceiverConfig.MAX_NUM_RECEIVERS = 4;

	car = new ElegooCarV3(carConfig);
	car->setup();
	car->registerInfraredConfig(new ElegooInfraredConfig());
	car->registerInfraredConfig(new PanasonicInfraredConfig());
	car->registerBluetoothConfig(new MyBluetoothConfig());

	// do some tests
	car->testDistanceUnit();
	//car->testInfrared();
	//car->testBluetooth();
	//car->testMotorUnit();
}

void loop()
{
	car->drive();
}
