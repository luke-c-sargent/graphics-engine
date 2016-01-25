#pragma once
#include <time.h>
#include "TimeKeeper.h"

class LinuxTime: public TimeKeeper {
	void get_current_time(timespec& tsr){
		clock_gettime(CLOCK_MONOTONIC, &tsr);
	}
public:
	LinuxTime(){
		init();
	}
	~LinuxTime(){}
};