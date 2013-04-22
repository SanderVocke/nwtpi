#ifndef _NATIVE_ELEMENT_H
#define _NATIVE_ELEMENT_H

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "bcm_host.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"

#include "NativeResource.h"

#ifdef DEBUG_ON
    #define DEBUG( a, b ) logd(a, ostringstream().flush() << b )
#else
    #define DEBUG( a, b )
#endif

using namespace std;

namespace nwtpi {

	enum NATIVE_ELEMENT_ENUM : unsigned int { SCREEN_ELEMENT=0, IMAGE_ELEMENT };

class NativeElement {
private:
#ifdef DEBUG_ON
	void logd(string , ostream& );
#endif

protected:
	static int					elementsCounter;
	int							elementId;

	DISPMANX_ELEMENT_HANDLE_T	elementHandle;
	DISPMANX_DISPLAY_HANDLE_T	displayHandle;
	DISPMANX_UPDATE_HANDLE_T	updateHandle;
	VC_RECT_T       			region;				// x, y, w, h			dest_rect
	VC_DISPMANX_ALPHA_T			alpha;
	unsigned int 				priority;
	const static unsigned int	defaultPriority;
	NativeResource * 			resource;
	EGL_DISPMANX_WINDOW_T		eglNativeWindowHandle;

	DISPMANX_ELEMENT_HANDLE_T	updateStart( int priority );
	bool						updateLockState;

	EGLSurface 					eglSurface;

	void 						setRegion(unsigned int x, unsigned y, unsigned int w, unsigned int h);
	void 						setAlpha(unsigned char _alpha);

	DISPMANX_ELEMENT_HANDLE_T 	add(DISPMANX_DISPLAY_HANDLE_T disp, DISPMANX_UPDATE_HANDLE_T upd,
									VC_RECT_T regionDst, VC_RECT_T regionSrc,
									VC_DISPMANX_ALPHA_T _alpha);
public:
	// create element from a void resource and cook update for EGLCreateWindowSurface
	NativeElement(DISPMANX_DISPLAY_HANDLE_T display, unsigned int x, unsigned y, unsigned int w, unsigned int h, unsigned char alpha);

	// w/ resource element
	NativeElement(DISPMANX_DISPLAY_HANDLE_T display, NativeResource resource,  unsigned int x, unsigned y, unsigned int w, unsigned int h, unsigned char alpha);

	~NativeElement();

	int							updateSync();
	int							updateSync( DISPMANX_UPDATE_HANDLE_T update );
	void						setUpdateLockState(bool lockState);
	int 						getId();
	VC_RECT_T					getRegion();
	DISPMANX_ELEMENT_HANDLE_T	getHandle();
	VC_DISPMANX_ALPHA_T			getAlpha();

	EGLNativeWindowType			getEglNativeWindowType();	// cooked Egl window handle elementHandle, width, height

};

}		// namespace nwtpi
#endif 	// _NATIVE_ELEMENT_H
