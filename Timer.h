#pragma once
#include "Timepoint.h"

class Timer{
	Timepoint _start;
	Timepoint stop(Timepoint t){return t-_start;};
	void reset(Timepoint t){_start=t;};
public:
	Timer(Timepoint t):_start(t){}
};