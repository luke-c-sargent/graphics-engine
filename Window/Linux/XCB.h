#pragma once
#include <xcb/xcb.h>
#include <string>

class XCB {
	typedef uint32_t xcb_window_t;

	uint32_t 			width, height;
	xcb_connection_t* 	connection;
	xcb_window_t		window;
	xcb_screen_t*		screen;


	std::string name = "Test XCB window! :D";

public:
	XCB(int, int);
	xcb_window_t get_window();
	xcb_connection_t* get_connection();
	void doIExist();
};