
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


