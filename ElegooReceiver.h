#ifndef __ELEGOO_RECEIVER_H__
#define __ELEGOO_RECEIVER_H__

class ElegooReceiver
{
public:
	virtual ElegooCommand readCommand() = 0;

	ElegooReceiver() {}

	virtual ~ElegooReceiver()
	{
	}

};

#endif
