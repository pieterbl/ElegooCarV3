
#ifndef __ELEGOO_INFRARED_CONFIG_INTERFACE_H__
#define __ELEGOO_INFRARED_CONFIG_INTERFACE_H__

#include "ElegooMoveCommand.h"

class ElegooInfraredConfigInterface
{
public:
	virtual ElegooMoveCommand checkCommand(unsigned long possibleCommandCode) = 0;

	virtual ~ElegooInfraredConfigInterface()
	{
	}
};


#endif
