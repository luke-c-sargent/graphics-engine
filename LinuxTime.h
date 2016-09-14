#pragma once
#include <time.h>
#include "TimeKeeper.h"

class LinuxTime: public TimeKeeper {
	void get_current_time(Timepoint& tpr){
		timespec tsr;
		clock_gettime(CLOCK_MONOTONIC, &tsr);
		tpr.seconds = (uint_fast32_t)tsr.tv_sec;
		tpr.nanos = (uint_fast32_t)tsr.tv_nsec;
	}
	Timepoint get_current_time(){
		timespec tsr;
		clock_gettime(CLOCK_MONOTONIC, &tsr);
		return Timepoint(tsr);
	}

public:
	LinuxTime(){}
	~LinuxTime(){}
};