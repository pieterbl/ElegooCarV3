#ifndef __ELEGOO_INTERRUPTIBLE_UNIT_H__
#define __ELEGOO_INTERRUPTIBLE_UNIT_H__

#include "ElegooCommand.h"
#include "ElegooCommandReader.h"

class ElegooInterruptibleUnit
{
private:
	ElegooCommandReader * commandReader;

protected:
	ElegooInterruptibleUnit() :
			commandReader(0)
	{
	}

public:
	void registerCommandReader(ElegooCommandReader * pCommandReader)
	{
		commandReader = pCommandReader;
	}

	bool hasCommand()
	{
		if (commandReader == 0)
		{
			return false;
		}

		return commandReader->hasCommand();
	}

};

#endif
