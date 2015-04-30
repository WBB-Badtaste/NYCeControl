#pragma once

#include "Axis_Base.h"
#include <string>

class Axis_Motion : virtual public Axis_Base
{
protected:
	BOOL        *m_pAxHandle;

	Axis_Motion(uint32_t num, const char* const names[], BOOL bSim);
	~Axis_Motion(void);
	void TermAxis();
	BOOL InitAxis();
};

