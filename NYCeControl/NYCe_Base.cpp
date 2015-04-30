#include "StdAfx.h"
#include "NYCe_Base.h"

NYCe_Base::NYCe_Base(BOOL bSim = FALSE)
{	
	if (bSim)
	{
		nyceStatus = NyceInit(NYCE_SIM);
	}
	else
	{
		nyceStatus = NyceInit(NYCE_NET);
	}
	if (NyceError(nyceStatus))
	{
		EHer.HandleError(nyceStatus, "NyceInit");
	}
};

NYCe_Base::~NYCe_Base(void)
{
	nyceStatus = NyceTerm();
	if (NyceError(nyceStatus))
	{
		EHer.HandleError(nyceStatus, "NyceInit");
	}
};