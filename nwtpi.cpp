#include <iostream>
#include <sstream>
#include <stdexcept>

#include "nwtpi.h"

// Static data initialization
bool 						NWTPI::bcInitiated = false;
bool 						NWTPI::dmDisplayOpened = false;
bool 						NWTPI::dmOpacified = true;
unsigned int 				NWTPI::bcDeviceWidth = 0;
unsigned int 				NWTPI::bcDeviceHeight = 0;
DISPMANX_DISPLAY_HANDLE_T 	NWTPI::dmDisplay = (unsigned int) 0;
DISPMANX_UPDATE_HANDLE_T 	NWTPI::dmUpdate = (unsigned int) 0;
DISPMANX_ELEMENT_HANDLE_T	NWTPI::dmElement = (unsigned int) 0;
EGL_DISPMANX_WINDOW_T		NWTPI::dmWindow = {};


//NWTPI::NWTPI (string title, unsigned int w, unsigned int h) : NWTPI (title, w, h, true) { } // XXX DELEGATED Constructor not supported on gcc 4.6

NWTPI::NWTPI(string title, unsigned int w, unsigned int h, bool opac, EGLCapabilities * caps)
	: windowTitle(title), windowWidth(w), windowHeight(h), egCapabilities(caps)
{

	if ( bcInitiated == false ) {

		dmOpacified = opac;

		bcm_host_init();
		bcInitiated=true;

		DEBUG ( "NWTPI", "bcmHostInit done." );

		if ( graphics_get_display_size( LCD, &bcDeviceWidth, &bcDeviceHeight) < 0)
			throw runtime_error("runtime error : NWTPI::NWTPI ** cannot get graphic_display_size.");

		DEBUG ( "NWTPI", "graphicsGetDisplaySize done : " << bcDeviceWidth << "," << bcDeviceHeight );

		if ( ( windowWidth > bcDeviceWidth ) || ( windowHeight > bcDeviceHeight ) )
			throw runtime_error("runtime error : NWTPI::NWTPI ** window sizes exceed device capabilities.");


		dmDisplay = dmDisplayOpen( LCD );											// set dmDisplay
		dmUpdate  = dmUpdateStart(0);												// dmUpdate
		dmElement = dmElementAdd(dmUpdate, dmDisplay, windowWidth, windowHeight);	// dmElement will fill the entire window

		DEBUG ( "NWTPI", " display : " << hex << dmDisplay << ", upd : " << hex << dmUpdate << ", elm : " << hex << dmElement << dec );

		dmUpdateSync(dmUpdate);														// validate our element we have just created.

		dmWindow.element = dmElement;
		dmWindow.width = windowWidth;
		dmWindow.height = windowHeight;

		if ( ( egContext = egCreateContext() ) == EGL_NO_CONTEXT )					// create eglDisplay,Context,Surface from dmWindow.element+w+h
			throw runtime_error("runtime error : NWTPI::NWTPI egCreateContext ** failed");

		if ( eglMakeCurrent(egDisplay, egSurface, egSurface, egContext) == EGL_FALSE )
				throw runtime_error("runtime error : NWTPI::egCreateContext ** eglMakeCurrent context failed.");

		DEBUG ("NWTPI" , "egCreateContext and make current done." );

	} else
		throw runtime_error("runtime error : NWTPI::NWTPI ** bcm host already initiated.");
}

NWTPI::~NWTPI() {
	// Release OpengGL resources
	eglMakeCurrent( egDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
	eglDestroySurface( egDisplay, egSurface );
	eglDestroyContext( egDisplay, egContext );
	eglTerminate( egDisplay );

	// then close vchiq
	if ( bcInitiated ) {
		dmDisplayClose();
		bcm_host_deinit();
	}
}

DISPMANX_DISPLAY_HANDLE_T NWTPI::dmDisplayOpen( unsigned int dev)
{
	DISPMANX_DISPLAY_HANDLE_T dmd;
	if ( ! dmDisplayOpened ) {
		dmd = vc_dispmanx_display_open( (uint32_t) dev );
		if ( dmd == DISPMANX_NO_HANDLE )
			throw runtime_error("runtime error : NWTPI::dmDisplayOpen ** does not return an handle");
	}
	dmDisplayOpened = true;

	return dmd;
}

int NWTPI::dmDisplayClose()
{
	return vc_dispmanx_display_close(dmDisplay);
}

void NWTPI::dmDisplayGetInfo( DISPMANX_DISPLAY_HANDLE_T disp, DISPMANX_MODEINFO_T info)
{
	vc_dispmanx_display_get_info(disp, &info);
}

DISPMANX_UPDATE_HANDLE_T NWTPI::dmUpdateStart( int prio )
{
	static DISPMANX_UPDATE_HANDLE_T upd;

	upd = vc_dispmanx_update_start( (int32_t) prio);
	if ( upd== DISPMANX_NO_HANDLE )
		throw runtime_error("runtime error : NWTPI::dmUpdateStart ** does not return an handle.");

	return upd;
}

DISPMANX_ELEMENT_HANDLE_T NWTPI::dmElementAdd(	DISPMANX_UPDATE_HANDLE_T upd,
												DISPMANX_DISPLAY_HANDLE_T disp,
												unsigned int w, unsigned int h)
{
	VC_RECT_T 			dmDstRect;
	VC_RECT_T 			dmSrcRect;
	VC_DISPMANX_ALPHA_T dmAlpha;

	dmDstRect.x = 0;
	dmDstRect.y = 0;
	dmDstRect.width = w;
	dmDstRect.height = h;

	dmSrcRect.x = 0;
	dmSrcRect.y = 0;
	dmSrcRect.width = w << 16;
	dmSrcRect.height = h << 16;


	memset(&dmAlpha, 0x0, sizeof(VC_DISPMANX_ALPHA_T));

	// whole window opacity is set as On or Off ;
	// when off, background or objects alpha value will be set later by GL
	//
	if (dmOpacified == true) {
		dmAlpha.flags = DISPMANX_FLAGS_ALPHA_FIXED_ALL_PIXELS;
		dmAlpha.opacity = 0xFF;
		dmAlpha.mask = 0;
	} else {
		dmAlpha.flags = DISPMANX_FLAGS_ALPHA_FROM_SOURCE;
		dmAlpha.opacity = 0xFF;
		dmAlpha.mask = 0xFF;
	}

	return vc_dispmanx_element_add (upd,						/* update 						*/
									disp,						/* display 						*/
									0,							/* int32_t layer				*/
									&dmDstRect,					/* dest_rect 					*/
									0,							/* src							*/
									&dmSrcRect,					/* src_rect 					*/
									DISPMANX_PROTECTION_NONE,	/* DISPMANX_PROTECTION 		TODO */
									&dmAlpha, 					/* VC_DISPMANX_ALPHA_T alpha     */
									0,							/* DISPMANX_CLAMP_T 		TODO */
									DISPMANX_NO_ROTATE);		/* DISPMANX_TRANSFORM_T 	TODO */
}

int NWTPI::dmUpdateSync(DISPMANX_UPDATE_HANDLE_T upd)
{
	return vc_dispmanx_update_submit_sync(upd);
}

EGLContext NWTPI::egCreateContext() {
	EGLint 		egMajorVersion;
	EGLint 		egMinorVersion;
	EGLint 		egNumConfigs;

	EGLNativeWindowType nativeWindow = &dmWindow;				// void * = &EGL_DISPMANX_WINDOW_T

//	static const EGLint egAttribList[] =						// TODO : refinements
//	{
//		EGL_RED_SIZE,       5,
//		EGL_GREEN_SIZE,     6,
//		EGL_BLUE_SIZE,      5,
//		EGL_ALPHA_SIZE,     (RGB & ALPHA) ? 8 : EGL_DONT_CARE,
//		EGL_DEPTH_SIZE,     (RGB & DEPTH) ? 8 : EGL_DONT_CARE,
//		EGL_STENCIL_SIZE,   (RGB & STENCIL) ? 8 : EGL_DONT_CARE,
//		EGL_SAMPLE_BUFFERS, (RGB & MULTISAMPLE) ? 1 : 0,
//		EGL_NONE
//	};

	EGLint * eglAttribList = egCapabilities->getAttributes();

	// Pi context => GLES/2 :                 v               v
	EGLint egContextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };

	if ( ( egDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY) ) == EGL_NO_DISPLAY )
		throw runtime_error("runtime error : NWTPI::egCreateContext ** EGL_NO_DISPLAY");

	if ( ! eglInitialize(egDisplay, &egMajorVersion, &egMinorVersion) )
		throw runtime_error("runtime error : NWTPI::egCreateContext ** eglInitialize failed.");

//	if ( ! eglGetConfigs(egDisplay, NULL, 0, &egNumConfigs) ) TODO
//		return -3;

	if ( eglChooseConfig(egDisplay, eglAttribList, &egConfig, 1, &egNumConfigs) == EGL_FALSE )
		throw runtime_error("runtime error : NWTPI::egCreateContext ** eglChooseConfig failed.");

	// TODO eglCreateWindowSurface (,,, attribList [EGL RENDER BUFFER, EGL VG COLORSPACE, and EGL VG ALPHA FORMAT.] ==> Khronos eglspecs-1.4 pp 26
	egSurface = eglCreateWindowSurface(egDisplay, egConfig, nativeWindow, NULL );
	if ( egSurface == EGL_NO_SURFACE )
		throw runtime_error("runtime error : NWTPI::egCreateContext ** EGL_NO_SURFACE");

	DEBUG ("NWTPI::egCreateContext","egConfig #" << egConfig );
	return ( eglCreateContext(egDisplay, egConfig, EGL_NO_CONTEXT, egContextAttribs ) );

}

unsigned int NWTPI::getWindowWidth() {
	return windowWidth;
}

unsigned int NWTPI::getWindowHeight() {
	return windowHeight;
}

string NWTPI::getWindowTitle() {
	return windowTitle;
}

void NWTPI::swapBuffers() {
	eglSwapBuffers(egDisplay, egSurface);
}

EGLDisplay NWTPI::getCurrentDisplay() {
	return egDisplay;
}

EGLSurface NWTPI::getCurrentSurface() {
	return egSurface;
}

#ifdef DEBUG_ON
void NWTPI::logd(string method, ostream& message) {
	ostringstream& s = dynamic_cast<ostringstream&>(message);
	cout << "==> " << method << " : " << s.str() << endl;
}
#endif
