#pragma once
struct Timepoint{
	uint_fast32_t _seconds;
	uint_fast32_t _nanos;

	friend Timepoint operator-(const Timepoint& lhs,const Timepoint& rhs){
		Timepoint now;
		now._seconds = lhs._seconds - rhs._seconds;
		now._nanos = lhs._nanos - rhs._nanos;
		return now;
	}
	void operator=(const Timepoint& rhs){
		_seconds=rhs._seconds;
		_nanos=rhs._nanos;
	}
	void print(){
		std::cout<<"TP:["<<_seconds<<" sec]["<<_nanos<<" ns]\n";
	}
};