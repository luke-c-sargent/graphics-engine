#pragma once
#include <iostream>
//X11
#include <X11/X.h>
#include <X11/Xlib.h>
//OpenGL
#include <GL/gl.h>
#include <GL/glx.h>

#include "GameWindow.h"

class LinuxWindow: GameWindow {
	//X11 variables
	Display*			 	display;
	GLint                   attributes[5] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
	XVisualInfo*			visual_info;
	Colormap				color_map;
	XSetWindowAttributes    frame_attributes;
	Window                  frame_window;
	Window 					root;
	GLXContext 				gl_context;
	XWindowAttributes 		gl_window_attributes;
	XEvent					xevent;

public:
	LinuxWindow(){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		display = XOpenDisplay(NULL);
		if(display == NULL){
			std::cout << "error in window formation\n";
			exit(0);
		}
		root = DefaultRootWindow(display);
		visual_info = glXChooseVisual(display, 0,attributes);
		if(visual_info == NULL){
			std::cout << "error with visual choice\n";
			exit(0);
		}
		color_map = XCreateColormap(display, root, visual_info->visual, AllocNone);
		frame_attributes.colormap=color_map;
		frame_attributes.event_mask= ExposureMask | KeyPressMask;
		frame_window = XCreateWindow(display, root, 0, 0, 600, 600, 0, visual_info->depth, InputOutput, visual_info->visual, CWColormap | CWEventMask, &frame_attributes);
		XMapWindow(display, frame_window);
		XStoreName(display,frame_window,"fnjork!");
		gl_context = glXCreateContext(display,visual_info,NULL,GL_TRUE);
		if(gl_context == NULL) {
			printf("\n\tcannot create gl context\n\n");
		exit(0); }
		glXMakeCurrent(display,frame_window,gl_context);
		glEnable(GL_DEPTH_TEST);

		 while(1) {
 			XNextEvent(display, &xevent);
        
     		if(xevent.type == Expose) {
     			glClearColor(1.0,0,1.0,1.0);
     			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        		glXSwapBuffers(display, frame_window);
        	}
                
			else if(xevent.type == KeyPress) {
        		glXMakeCurrent(display, None, NULL);
 				glXDestroyContext(display, gl_context);
		 		XDestroyWindow(display, frame_window);
		 		XCloseDisplay(display);
		 		exit(0); } 
                
		} /* while(1) */
	}
};