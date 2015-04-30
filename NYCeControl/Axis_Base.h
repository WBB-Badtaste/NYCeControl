#pragma once

#include "NYCe_Base.h"
#include <sacapi.h>
class Axis_Base : virtual public NYCe_Base
{
private:
	BOOL *m_pAxCon;
	BOOL ConnectAxes();
	BOOL DisconnectAxes();

protected:
	uint32_t m_axNum;
	SAC_AXIS *m_pAxId;
	char **m_pAxName;

	Axis_Base(uint32_t num, const char* const names[], BOOL bSim);
	~Axis_Base(void);

public:
	BOOL Reconnect();
};

