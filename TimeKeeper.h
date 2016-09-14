#pragma once
#include <time.h>
#include <stdint.h>
#include <iostream>

#define BILLION 1000000000

class Timepoint{
public:
	uint_fast32_t seconds;
	uint_fast32_t nanos;

	friend Timepoint operator-(const Timepoint& lhs,const Timepoint& rhs){
		uint_fast32_t sec=0;
		uint_fast32_t ns=0;

		sec = lhs.seconds - rhs.seconds;
		if (lhs.nanos < rhs.nanos){
			ns = (BILLION + lhs.nanos) - rhs.nanos;
			sec -= 1;
		}
		else
			ns = lhs.nanos - rhs.nanos;
		return Timepoint(sec, ns);
	}
	Timepoint(uint_fast32_t _sec, uint_fast32_t _nsec): seconds(_sec), nanos(_nsec){}
	Timepoint(const timespec& ts): seconds(ts.tv_sec), nanos(ts.tv_nsec){}
	Timepoint(){}
};

class TimeKeeper{

	Timepoint start;
public:
	virtual void get_current_time(Timepoint& )=0;
	virtual Timepoint get_current_time()=0;

	void print_time(){
		Timepoint now = get_current_time();
		std::cout << now.nanos-start.nanos << " ns" << std::endl;
	}

	Timepoint elapsed(){
		return  get_current_time() - start; 
	}
//	TimeKeeper():start(get_current_time()){}

	virtual ~TimeKeeper(){}
};