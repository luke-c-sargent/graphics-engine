#include "XCB.h"
#include <xcb/xcb.h>
#include <iostream>
#include <assert.h>

XCB::XCB(int _width, int _height){
  int32_t screen_index;
  connection = xcb_connect (NULL, &screen_index);

  std::cout << "creating XCB window " << _width << "x" << _height << " at screen index " << 
      screen_index << " at address " <<  connection << std::endl;

  // error check
	std::cout << "XCB ERROR CODE: " << xcb_connection_has_error(connection) << std::endl;

  /* Get the first screen */
  const xcb_setup_t      *setup  = xcb_get_setup (connection);
  xcb_screen_iterator_t   screen_iterator   = xcb_setup_roots_iterator (setup);

  // get primary screen
  while( screen_index-- > 0 ) {
    xcb_screen_next( &screen_iterator );
  }//*/
  xcb_screen_t           *screen = screen_iterator.data;

  /* Create the window */
  window = xcb_generate_id (connection);
  std::cout << "[XCB] Window: " << window << std::endl;

  /* event masks */
  uint32_t event_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
  //uint32_t value_list[] = {screen->black_pixel, 0 };
  uint32_t value_list[] = {
    screen->black_pixel, 
    XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_STRUCTURE_NOTIFY 
  };


  xcb_generic_error_t *err;
  xcb_void_cookie_t ck;

  ck = xcb_create_window_checked (
                     connection,                    /* Connection          */
                     XCB_COPY_FROM_PARENT,          /* depth (same as root)*/
                     window,                        /* window Id           */
                     screen->root,                  /* parent window       */
                     10, 10,                          /* x, y                */
                     _width, _height,                      /* width, height       */
                     0,                            /* border_width        */
                     XCB_WINDOW_CLASS_INPUT_OUTPUT, /* class               */
                     screen->root_visual,           /* visual              */
                     event_mask, value_list );      /* masks				 */


  err = xcb_request_check (connection, ck);
  if (err != NULL)
    std::cout << "X11 Error " << err->error_code;
  else
    std::cout << "XCB window creation <evidently> good" << std::endl;
//xcb_flush (connection);

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
  // TO BE DEFERRED
  //const uint32_t coords[] = {100, 100};
  //xcb_configure_window(connection, window, XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y, coords);

  xcb_flush (connection);
  /* Make sure commands are sent before we pause so that the window gets shown */
  //while(true){}
  /*xcb_generic_event_t *e;
  while ((e = xcb_wait_for_event(connection))) {
      if ((e->response_type & ~0x80) == XCB_EXPOSE)
          break;
  }*/

  std::cout << "end of creation connection: " << connection << std::endl;
}

xcb_window_t XCB::get_window(){
  return window;
}

xcb_connection_t* XCB::get_connection(){
  assert(connection);
  return connection;
}

void XCB::doIExist(){std::cout << "I EXIST DEMMIT at conn addr " << connection << std::endl;}