#ifndef __ELEGOO_BASE_H__
#define __ELEGOO_BASE_H__

#include "ElegooConstants.h"

class ElegooBase
{
public:
	ElegooBase()
	{
	}

	int statusOK()
	{
		return ElegooConstants::OK;
	}
};

#endif
