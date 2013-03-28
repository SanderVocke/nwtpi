#include <iostream>
#include <stdexcept>
#include "nwtpi.h"

// Static data initialization
bool 						NWTPI::bcInitiated = false;
bool 						NWTPI::dmDisplayOpened = false;
unsigned int 				NWTPI::bcDeviceWidth = 0;
unsigned int 				NWTPI::bcDeviceHeight = 0;
DISPMANX_DISPLAY_HANDLE_T 	NWTPI::dmDisplay = (unsigned int) 0;
DISPMANX_UPDATE_HANDLE_T 	NWTPI::dmUpdate = (unsigned int) 0;
DISPMANX_ELEMENT_HANDLE_T	NWTPI::dmElement = (unsigned int) 0;
EGL_DISPMANX_WINDOW_T		NWTPI::dmWindow = {};


NWTPI::NWTPI(char* title, unsigned int winWidth, unsigned int winHeight) {
	int cr = 0;

	if ( bcInitiated == false ) {

		bcm_host_init();
		bcInitiated=true;

		cout << "NWTPI ==> bcmHostInit done." << endl;

		if ( graphics_get_display_size( LCD, &bcDeviceWidth, &bcDeviceHeight) < 0)
			throw runtime_error("runtime error : NWTPI::NWTPI ** cannot get graphic_display_size.");

		cout << "NWTPI ==> graphicsGetDisplaySize done : " << bcDeviceWidth << "," << bcDeviceHeight << endl;

		if ( ( winWidth > bcDeviceWidth ) || ( winHeight > bcDeviceHeight ) )
			throw runtime_error("runtime error : NWTPI::NWTPI ** window sizes exceed device capabilities.");


		dmDisplay = dmDisplayOpen( LCD );											// set dmDisplay
		dmUpdate  = dmUpdateStart(0);												// dmUpdate
		dmElement = dmElementAdd(dmUpdate, dmDisplay, winWidth, winHeight);			// dmElement will fill the entire window
		cout << "==> NWTPI:NWTPI : display : " << hex << dmDisplay
								 << ", upd : " << hex << dmUpdate
								 << ", elm : " << hex << dmElement
								 << endl;
		dmUpdateSync(dmUpdate);														// validate our element we have just created.

		dmWindow.element = dmElement;
		dmWindow.width = winWidth;
		dmWindow.height = winHeight;

		if ( ( cr = egCreateContext() ) < 0 ) {													// create eglDisplay,Context,Surface from dmWindow.element+w+h
			cout << "[" << dec << cr << "]" << endl;
			throw runtime_error("runtime error : NWTPI::NWTPI.egCreateContext ** failed : ");
		}
		cout << "NWTPI ==> egCreateContext done." << endl;

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
	VC_RECT_T dmDstRect;
	VC_RECT_T dmSrcRect;

	dmDstRect.x = 0;
	dmDstRect.y = 0;
	dmDstRect.width = w;
	dmDstRect.height = h;

	dmSrcRect.x = 0;
	dmSrcRect.y = 0;
	dmSrcRect.width = w << 16;
	dmSrcRect.height = h << 16;

	return vc_dispmanx_element_add (upd,
									disp,
									0,							/* layer						*/
									&dmDstRect,
									0,							/* src							*/
									&dmSrcRect,
									DISPMANX_PROTECTION_NONE,	/* DISPMANX_PROTECTION define   TODO */
									0, 							/* VC_DISPMANX_ALPHA_T alpha 	TODO */
									0,							/* DISPMANX_CLAMP_T 			TODO */
									DISPMANX_NO_ROTATE);		/* enum DISPMANX_TRANSFORM_T 	TODO */
}

int NWTPI::dmUpdateSync(DISPMANX_UPDATE_HANDLE_T upd)
{
	return vc_dispmanx_update_submit_sync(upd);
}

int NWTPI::egCreateContext() {

	EGLint 		egMajorVersion;
	EGLint 		egMinorVersion;
	EGLint 		egNumConfigs;

	EGLNativeWindowType nativeWindow = &dmWindow;				// void * = &EGL_DISPMANX_WINDOW_T

	static const EGLint egAttribList[] =						// TODO : refinements
	{
		EGL_RED_SIZE,       5,
		EGL_GREEN_SIZE,     6,
		EGL_BLUE_SIZE,      5,
		EGL_ALPHA_SIZE,     (RGB & ALPHA) ? 8 : EGL_DONT_CARE,
		EGL_DEPTH_SIZE,     (RGB & DEPTH) ? 8 : EGL_DONT_CARE,
		EGL_STENCIL_SIZE,   (RGB & STENCIL) ? 8 : EGL_DONT_CARE,
		EGL_SAMPLE_BUFFERS, (RGB & MULTISAMPLE) ? 1 : 0,
		EGL_NONE
	};

	// Pi context => GLES/2 :                 v               v
	EGLint egContextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };

	//if ( ( egDisplay = eglGetDisplay((EGLNativeDisplayType) NULL) ) == EGL_NO_DISPLAY )
	if ( ( egDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY) ) == EGL_NO_DISPLAY )
		return -1;

	if ( ! eglInitialize(egDisplay, &egMajorVersion, &egMinorVersion) )
		return -2;

//	if ( ! eglGetConfigs(egDisplay, NULL, 0, &egNumConfigs) ) TODO
//		return -3;

	if ( eglChooseConfig(egDisplay, egAttribList, &egConfig, 1, &egNumConfigs) == EGL_FALSE )
		return -4;

	// TODO eglCreateWindowSurface (,,, attribList [EGL RENDER BUFFER, EGL VG COLORSPACE, and EGL VG ALPHA FORMAT.] ==> pp 26
	egSurface = eglCreateWindowSurface(egDisplay, egConfig, nativeWindow, NULL );
	if ( egSurface == EGL_NO_SURFACE )
		return -5;

	egContext = eglCreateContext(egDisplay, egConfig, EGL_NO_CONTEXT, egContextAttribs );
	if ( egContext == EGL_NO_CONTEXT )
		return -6;

	if ( eglMakeCurrent(egDisplay, egSurface, egSurface, egContext) == EGL_FALSE )
		return -7;

	return 1;
}

void NWTPI::swapBuffers() {
	eglSwapBuffers(egDisplay, egSurface);
}
