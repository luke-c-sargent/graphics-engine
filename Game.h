#include <string>
#include <iostream>
#include "Timer.h"
#include "TimeKeeper.h"
#include "LinuxTime.h"

class Game{
public:

	TimeKeeper* tkp;

	// enums
	enum Platform: unsigned char
	{
		LINUX=0,
		WINDOWS,
		NO_PLATFORM
	};
	std::string platform_strings[3] = {"Linux","Windows","Detection Error"};

	enum Architecture: unsigned char
	{
		X32=0, 
		X64, 
		NO_ARCH
	};
	std::string architecture_strings[3] = {"32-bit","64-bit","Detection Error"};
	
	// variables 
	Platform platform;
	Architecture architecture;

	//functions
	Game();

private:
	//TimeKeeper* tkp;
	void sense_local_environment();
	void init();
	void main_loop();
};