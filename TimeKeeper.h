#pragma once
#include <time.h>
#include <stdint.h>
#include <iostream>
#include "Timepoint.h"
#include "Timer.h"

class TimeKeeper{
	Timepoint start;
protected:
	void init();
public:
	virtual Timepoint get_current_time()=0;
	void print_time();
	Timer start_timer();
	virtual ~TimeKeeper(){};
};