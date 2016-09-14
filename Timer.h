#pragma once

#include <time.h>
#include "TimeKeeper.h"

class Timer{
	static TimeKeeper* _tkp;
	Timepoint _start;

public:
	void start(){_start=Timer::_tkp->get_current_time();}
	Timepoint stop(){return _start - Timer::_tkp->get_current_time();};
	void reset(){start();};
	Timer(TimeKeeper* tkp):
		_start(tkp->get_current_time()){
			Timer::_tkp = tkp;
		}
};