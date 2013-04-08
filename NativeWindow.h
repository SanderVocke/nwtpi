#ifndef _NATIVE_WINDOW_H
#define _NATIVE_WINDOW_H

#include <iostream>
#include <sstream>

#include "bcm_host.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"


#ifdef DEBUG_ON
    #define DEBUG( a, b ) logd(a, ostringstream().flush() << b )
#else
    #define DEBUG( a, b )
#endif

using namespace std;


class NativeWindow {

private:
	//
	// BCM low Level
	//
	static unsigned int deviceWidth;									// bcm : those returned by graphic_get_display_size
	static unsigned int deviceHeight;									//
	static bool 		bcmInitiated;									// bcm_host_init() already called ?

	//
	// Native Display Manager (DISPMANX)
	//
	enum NW_DEVICE_ENUM : unsigned int { LCD=0, AUX_LCD, HDMI, SDTV };	// bcm devices number (vc_dispmanx_type.h)
	static bool 						displayOpened;
	static bool							opaqueness;
	static DISPMANX_DISPLAY_HANDLE_T	displayHandle;
	static DISPMANX_UPDATE_HANDLE_T		updateHandle;
	static DISPMANX_ELEMENT_HANDLE_T 	elementHandle;					// ->  dmWindow.element
	static DISPMANX_MODEINFO_T			modeInfo;						// width, height, VC_IMAGE_TRANSFORM_T transform, DISPLAY_INPUT_FORMAT_T input_format

	DISPMANX_DISPLAY_HANDLE_T 			displayOpen( unsigned int );
	int 								displayClose();
	void 								displayGetInfo( DISPMANX_DISPLAY_HANDLE_T display, DISPMANX_MODEINFO_T info);
	DISPMANX_UPDATE_HANDLE_T 			updateStart( int priority );
	DISPMANX_ELEMENT_HANDLE_T			elementAdd(DISPMANX_UPDATE_HANDLE_T upd, DISPMANX_DISPLAY_HANDLE_T disp, unsigned int w, unsigned int h);
	int 								updateSync(DISPMANX_UPDATE_HANDLE_T update);

#ifdef DEBUG_ON
	void logd(string , ostream& );
#endif

	unsigned int windowWidth;
	unsigned int windowHeight;

protected:
	static EGL_DISPMANX_WINDOW_T 		windowHandle;					// dispman eglplatform.h -- struct : width, height, element

	NativeWindow(unsigned int w, unsigned int h, bool opac);
	~NativeWindow();

	inline unsigned int		getNativeWindowWidth() 		{ return windowWidth; }
	inline unsigned int 	getNativeWindowHeight()		{ return windowHeight; }
	inline bool				getNativeWindowOpaqueness() { return opaqueness; }

	// TODO getters, setters ....

	//EGL_DIPMANX_WINDOW_T	getNativeWindowHandle(); TODO private windowHandle, protected getter getNativeWindowHandle() ...
};

#endif	// _NATIVE_WINDOW_H
