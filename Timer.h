#include <time.h>
#include "TimeKeeper.h"

class Timer{
	timespec _start;
	static TimeKeeper* _tkp=0;
	start(){_start=tkp->get_time();}
	stop(){return };
	void reset();
public:
	Timer(TimeKeeper* tkp):_tkp(tkp){}
}