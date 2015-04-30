// NYCeControl.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include "Axis_Control.h"

#define NUM_AXES 4
const char *axName[ NUM_AXES ] = { "DEF_AXIS_1", "DEF_AXIS_2", "DEF_AXIS_3", "DEF_AXIS_4"};

int _tmain(int argc, _TCHAR* argv[])
{
	Axis_Control ac((uint32_t)NUM_AXES, axName, TRUE);
}

