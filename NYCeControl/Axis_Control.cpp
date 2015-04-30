#include "StdAfx.h"
#include "Axis_Control.h"


Axis_Control::Axis_Control(uint32_t num, const char* const names[], BOOL bSimMode) : 
															Axis_Motion(num, names, bSimMode), 
															Axis_Base(num, names, bSimMode), 
															NYCe_Base(bSimMode)
{

}


Axis_Control::~Axis_Control(void)
{

}
