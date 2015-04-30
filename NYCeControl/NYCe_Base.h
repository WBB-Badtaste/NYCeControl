#pragma once

#include "ErrorHandler.h"
#include <nyceapi.h>

class NYCe_Base
{
protected:
	ErrorHandler EHer;
	NYCE_STATUS nyceStatus;

	NYCe_Base(BOOL bSim);
	~NYCe_Base(void);
};

