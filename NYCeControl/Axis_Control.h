#pragma once
#include "axis_motion.h"
class Axis_Control : public Axis_Motion
{
public:
	Axis_Control(uint32_t num, const char* const names[], BOOL bSimMode);
	~Axis_Control(void);
};

