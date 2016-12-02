#pragma once
#include "../../GameWindow.h"
#include <xcb/xcb.h>
#include <string>

class XCB : GameWindow {
	typedef uint32_t xcb_window_t;

	xcb_connection_t* 	connection;
	xcb_window_t		window;
	xcb_screen_t*		screen;

	std::string name = "Test XCB window! :D";

public:
	XCB();
	XCB(int, int);
	xcb_window_t get_window();
	xcb_connection_t* get_connection();
};