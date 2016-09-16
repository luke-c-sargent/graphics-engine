#include "Game.h"
#include "Timer.h"
//constructor
Game::Game(){
	std::cout << "Game constructing..." << std::endl;
	init();
}


void Game::sense_local_environment(){

// get platform
#if _WIN32
	platform = WINDOWS;
#elif __GNUC__
	platform = LINUX;
#else
	platform = NO_PLATFORM;
#endif

// get architecture
#if _WIN64
	architecture = X64;
#elif _WIN32
	architecture = X32;
#elif __x86_64__ || __ppc64__
	architecture = X64;
#else
	architecture = NO_ARCH;
#endif
}

void Game::init(){
	// detect platform and architecture
	sense_local_environment();
		
	std::cout << "Platform: " << platform_strings[platform] << std::endl;
	std::cout << "Architecture: " << architecture_strings[architecture] << std::endl;

	if(platform != LINUX){
		std::cout << "Linux only at the moment";
		exit(0);
	}

	switch(platform){
		case(LINUX):
			tkp = new LinuxTime();
			break;
		default:
			std::cout << "error\n";
	}
}

void Game::main_loop(){
	uint32_t frames = 0;
	Timer t(tkp);
	
	Timepoint _last=tkp->get_current_time();
	Timepoint _current = _last;
	//forever and ever
	while(true){
		_current = tkp->get_current_time();
		++frames;	
		if(t.elapsed().seconds >= 1 ){
			t.reset();
			printf("%u\n", frames);
			frames = 0;
		}

		//end execution
		_last = _current;
	}
}