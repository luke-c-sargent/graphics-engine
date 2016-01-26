#include <iostream>
#include "TimeKeeper.h"

void TimeKeeper::init(){
	start=get_current_time();
	//std::cout << "this happened" << std::endl;
}

void TimeKeeper::print_time(){
		{get_current_time().print();}
}

Timer TimeKeeper::start_timer(){
	return Timer(get_current_time());
}