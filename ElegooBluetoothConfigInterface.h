
#ifndef __ELEGOO_BLUETOOTH_CONFIG_INTERFACE_H__
#define __ELEGOO_BLUETOOTH_CONFIG_INTERFACE_H__

#include "Arduino.h"
#include "ElegooMoveCommand.h"

class ElegooBluetoothConfigInterface
{
public:
	virtual ElegooMoveCommand checkCommand(const char possibleCommandCode) = 0;

	virtual const char * getSupportedCharacters() = 0;

	virtual bool isSupportedCharacter(char charToCheck)
	{
		const char * supportedChars = this->getSupportedCharacters();
		const char * charFound = strchr(supportedChars, charToCheck);
		return (charFound != NULL);
	}

	virtual ~ElegooBluetoothConfigInterface()
	{
	}

};

#endif
