#include "XCB.h"
#include <xcb/xcb.h>

XCB::XCB(){
  XCB(800,600);
}

XCB::XCB(int _width, int _height){
	connection = xcb_connect (NULL, NULL);

	// error check
	if(xcb_connection_has_error(connection))
		exit(1);

    /* Get the first screen */
    const xcb_setup_t      *setup  = xcb_get_setup (connection);
    xcb_screen_iterator_t   iter   = xcb_setup_roots_iterator (setup);
    xcb_screen_t           *screen = iter.data;

    /* Create the window */
    xcb_window_t window = xcb_generate_id (connection);

    /* event masks */
    uint32_t event_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    uint32_t value_list[] = {screen->black_pixel, 0 };

    xcb_create_window (connection,                    /* Connection          */
                       XCB_COPY_FROM_PARENT,          /* depth (same as root)*/
                       window,                        /* window Id           */
                       screen->root,                  /* parent window       */
                       10, 10,                          /* x, y                */
                       _width, _height,                      /* width, height       */
                       0,                            /* border_width        */
                       XCB_WINDOW_CLASS_INPUT_OUTPUT, /* class               */
                       screen->root_visual,           /* visual              */
                       event_mask, value_list );      /* masks				 */


    xcb_change_property(
    	connection,
    	XCB_PROP_MODE_REPLACE,
    	window,
    	XCB_ATOM_WM_NAME,
    	XCB_ATOM_STRING,
    	8,
    	name.size(),
    	name.c_str());

    /* Map the window on the screen */
    xcb_map_window (connection, window);


    /* Make sure commands are sent before we pause so that the window gets shown */
    xcb_flush (connection);
    //while(true){}
}

xcb_window_t XCB::get_window(){
  return window;
}

xcb_connection_t* XCB::get_connection(){
  return connection;
}