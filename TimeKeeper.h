#pragma once
#include <time.h>
#include <stdint.h>
#include <iostream>

class TimeKeeper{
	struct Timepoint{
		uint_fast32_t seconds;
		uint_fast64_t nanos;

		friend Timepoint operator-(const Timepoint& lhs,const Timepoint& rhs){
			Timepoint now;
			now.seconds = lhs.seconds - rhs.seconds;
			now.nanos = lhs.nanos - rhs.nanos;
			return now;
		}
	};

	Timepoint start;
	virtual void get_current_time(Timepoint& )=0;
protected:
	void init();
public:
	void print_time(){
		Timepoint now;
		get_current_time(now);
		uint_fast64_t now_ns=now.tv_nsec;
		uint_fast64_t start_ns=start.tv_nsec;
		std::cout << now_ns-start_ns << " ns" << std::endl;
	}

	virtual ~TimeKeeper(){};
};