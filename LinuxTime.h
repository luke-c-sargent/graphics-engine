#pragma once
#include <time.h>
#include "TimeKeeper.h"

class LinuxTime: public TimeKeeper {
	Timepoint get_current_time(){
		timespec tsr;
		clock_gettime(CLOCK_MONOTONIC, &tsr);
		Timepoint tp;
		tp._seconds = tsr.tv_sec;
		tp._nanos = tsr.tv_nsec;
		return tp;
	}
public:
	LinuxTime(){
		init();
	}
	~LinuxTime(){}
};