#pragma once

#include <time.h>
#include "TimeKeeper.h"

class Timer{
	TimeKeeper* _tkp;
	Timepoint _start;

public:
	void start(){_start=_tkp->get_current_time();}
	
	Timepoint elapsed(){return _tkp->get_current_time() - _start;};
	
	void reset(){start();}
	
	Timer(TimeKeeper* tkp):
		_start(tkp->get_current_time()){
			Timer::_tkp = tkp;
		}
};