#pragma once
#include <iostream>
#include <utility>

void debugmsg(){
	std::cout<<std::endl;
}

template<typename First, typename ...Rest>
void debugmsg(First && first, Rest && ...rest){
	#ifdef DEBUGMSGS
	std::cout << std::forward<First>(first) << " ";
	debugmsg(std::forward<Rest>(rest)...);
	#endif
}