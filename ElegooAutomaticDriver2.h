#ifndef __ELEGOO_AUTOMATIC_DRIVER_2_H__
#define __ELEGOO_AUTOMATIC_DRIVER_2_H__

#include "ElegooConstants.h"
#include "ElegooDriverBase.h"
#include "ElegooDistanceUnit.h"
#include "ElegooMotorUnit.h"

class ElegooAutomaticDriver2: public ElegooDriverBase
{
private:
	int safetyDistanceInCM;
	ElegooDistanceUnit & distUnit;
	ElegooMotorUnit & motorUnit;

public:
	ElegooAutomaticDriver2( //
			int pSafetyDistanceInCM, ElegooDistanceUnit & pDistUnit, ElegooMotorUnit & pMotorUnit) :
			safetyDistanceInCM(pSafetyDistanceInCM), distUnit(pDistUnit), motorUnit(pMotorUnit)
	{
	}

	virtual ~ElegooAutomaticDriver2()
	{
	}

	virtual int processCommand(ElegooCommand cmd)
	{
		return ElegooConstants::OK;
	}

private:
};

#endif
