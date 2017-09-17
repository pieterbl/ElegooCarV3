#ifndef __ELEGOO_INFRARED_CONFIG_INTERFACE_H__
#define __ELEGOO_INFRARED_CONFIG_INTERFACE_H__

#include "ElegooCommand.h"

class ElegooInfraredConfigInterface
{
public:
	virtual ElegooCommand checkCommand(unsigned long possibleCommandCode) = 0;

	virtual ~ElegooInfraredConfigInterface()
	{
	}
};

#endif
