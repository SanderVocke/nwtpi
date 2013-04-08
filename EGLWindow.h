#ifndef _EGL_WINDOW_H
#define _EGL_WINDOW_H

#include <iostream>
#include <sstream>

#include "EGL/egl.h"
#include "EGL/eglext.h"
#include "GLES2/gl2.h"

#include "NativeWindow.h"
#include "EGLCapabilities.h"

#ifdef DEBUG_ON
    #define DEBUG( a, b ) logd(a, ostringstream().flush() << b )
#else
    #define DEBUG( a, b )
#endif

using namespace std;


class EGLWindow : protected NativeWindow {

private:
	string 			windowTitle;

	EGLCapabilities* capabilities;

	EGLDisplay		egDisplay;
	EGLConfig 		egConfig;
	EGLint			egConfigID;
	EGLSurface		egSurface;
	EGLContext		egContext;
	//EGLClientBuffer 	clientBuffer;	TODO

	EGLContext		createContext();
	//bool  		egCreateSurface();	TODO
	//bool  		egCreateWindow();	TODO
	//bool  		egCreateConfig();	TODO

	#ifdef DEBUG_ON
	void logd(string , ostream& );
#endif

public:
	// NWTPI (string, unsigned int, unsigned int);	XXX waiting for gcc4.7 ( http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2006/n1986.pdf )
	EGLWindow (string, unsigned int, unsigned int, bool, EGLCapabilities *);
	~EGLWindow ();

	string		 	getWindowTitle();
	unsigned int 	getWindowWidth();
	unsigned int 	getWindowHeight();

	EGLDisplay		getCurrentDisplay();
	EGLSurface		getCurrentSurface();
//	EGLContext*		getCurrentContext();			TODO
	void 			swapBuffers();

	EGLint			getEgConfigID();

};

#endif
