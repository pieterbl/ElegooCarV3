#ifndef __ELEGOO_AUTOMATIC_DRIVER_2_H__
#define __ELEGOO_AUTOMATIC_DRIVER_2_H__

#include "ElegooConstants.h"
#include "ElegooDriverBase.h"

class ElegooAutomaticDriver2: public ElegooDriverBase
{
private:
	ElegooCarV3 & car;

public:

	virtual ~ElegooAutomaticDriver2()
	{
	}

	virtual int processCommand(ElegooMoveCommand command)
	{
		return ElegooConstants::OK;
	}

private:
};

#endif
