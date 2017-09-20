#ifndef __ELEGOO_LINE_TRACKING_DRIVER_H__
#define __ELEGOO_LINE_TRACKING_DRIVER_H__

#include "ElegooDriverBase.h"

class ElegooLineTrackingDriver: public ElegooDriverBase
{
public:
	ElegooLineTrackingDriver(int pSafetyDistanceInCM, ElegooDistanceUnit & pDistUnit, ElegooMotorUnit & pMotorUnit) :
			ElegooDriverBase(pSafetyDistanceInCM, pDistUnit, pMotorUnit)
	{
	}

	virtual ~ElegooLineTrackingDriver()
	{
	}

	virtual int processCommand(ElegooCommand cmd)
	{
		return motorUnit.statusOK();
	}
};

#endif
