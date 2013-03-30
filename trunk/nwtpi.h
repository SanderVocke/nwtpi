#ifndef _NWTPI_H
#define _NWTPI_H

#include <iostream>
#include <sstream>

#include "bcm_host.h"

#include "EGL/egl.h"
#include "EGL/eglext.h"
#include "GLES2/gl2.h"

#ifdef DEBUG_ON
    #define DEBUG( a, b ) logd(a, ostringstream().flush() << b )
#else
    #define DEBUG( a, b )
#endif

using namespace std;


class NWTPI {

private:
	//
	// BCM low Level
	//
	static unsigned int bcDeviceWidth;									// bcm : those returned by graphic_get_display_size
	static unsigned int bcDeviceHeight;									//
	static bool 		bcInitiated;									// bcm_host_init() already called ?

	//
	// Native Display Manager (DISPMANX)
	//
	enum DM_DEVICE : unsigned int { LCD=0, AUX_LCD, HDMI, SDTV };		// bcm devices number (vc_dispmanx_type.h)
	static bool 						dmDisplayOpened;
	static DISPMANX_DISPLAY_HANDLE_T	dmDisplay;
	static DISPMANX_UPDATE_HANDLE_T		dmUpdate;
	static DISPMANX_ELEMENT_HANDLE_T 	dmElement;						// ->  dmWindow.element
	static DISPMANX_MODEINFO_T			dmModeInfo;						// width, height, VC_IMAGE_TRANSFORM_T transform, DISPLAY_INPUT_FORMAT_T input_format

	static EGL_DISPMANX_WINDOW_T 		dmWindow;						// dispman eglplatform.h -- struct : width, height, element

	DISPMANX_DISPLAY_HANDLE_T 			dmDisplayOpen( unsigned int );
	int 								dmDisplayClose();
	void 								dmDisplayGetInfo( DISPMANX_DISPLAY_HANDLE_T display, DISPMANX_MODEINFO_T info);
	DISPMANX_UPDATE_HANDLE_T 			dmUpdateStart( int priority );
	DISPMANX_ELEMENT_HANDLE_T			dmElementAdd(DISPMANX_UPDATE_HANDLE_T upd, DISPMANX_DISPLAY_HANDLE_T disp, unsigned int w, unsigned int h);
	int 								dmUpdateSync(DISPMANX_UPDATE_HANDLE_T update);

	//
	// EGL Stuff
	//
	enum ES_WINDOW_BUFFER_TYPE : unsigned int { RGB=0, ALPHA=1, DEPTH=2, STENCIL=4, MULTISAMPLE=8 };

#ifdef DEBUG_ON
	void logd(string , ostream& );
#endif

private:
	string 			windowTitle;
	unsigned int	windowWidth;
	unsigned int 	windowHeight;

	EGLDisplay		egDisplay;
	EGLConfig 		egConfig;
	EGLSurface		egSurface;
	EGLContext		egContext;
	//EGLClientBuffer 	clientBuffer;	TODO

	int 			egCreateContext();
	//bool  		egCreateSurface();	TODO
	//bool  		egCreateWindow();	TODO
	//bool  		egCreateConfig();	TODO

public:
	NWTPI (string, unsigned int, unsigned int);
	~NWTPI ();

	string		 	getWindowTitle();
	unsigned int 	getWindowWidth();
	unsigned int 	getWindowHeight();

	EGLDisplay		getCurrentDisplay();
	EGLSurface		getCurrentSurface();
//	EGLContext*		getCurrentContext();			TODO
	void 			swapBuffers();

	//EGLSurface 		getSurface();

};

#endif
