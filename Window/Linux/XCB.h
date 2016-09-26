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

	/*
	xcb_atom_t wmProtocols;
	xcb_atom_t wmDeleteWin;

*/
public:
	XCB(int, int);
};