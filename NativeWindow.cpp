#include <iostream>
#include <sstream>
#include <stdexcept>

#include "NativeWindow.h"

// Static data initialization
bool 						NativeWindow::bcmInitiated = false;
bool 						NativeWindow::displayOpened = false;
bool 						NativeWindow::opaqueness = true;
unsigned int 				NativeWindow::deviceWidth = 0;
unsigned int 				NativeWindow::deviceHeight = 0;
DISPMANX_DISPLAY_HANDLE_T 	NativeWindow::displayHandle = (unsigned int) 0;
DISPMANX_UPDATE_HANDLE_T 	NativeWindow::updateHandle  = (unsigned int) 0;
DISPMANX_ELEMENT_HANDLE_T	NativeWindow::elementHandle = (unsigned int) 0;
EGL_DISPMANX_WINDOW_T		NativeWindow::windowHandle = { (unsigned int) 0, 0, 0 };

NativeWindow::NativeWindow(unsigned int w, unsigned int h, bool opac)
	: windowWidth(w), windowHeight(h)
{

	if ( bcmInitiated == false ) {

		opaqueness = opac;

		bcm_host_init();
		bcmInitiated=true;

		DEBUG ( "NativeWindow", "bcmHostInit done." );

		if ( graphics_get_display_size( LCD, &deviceWidth, &deviceHeight) < 0)
			throw runtime_error("runtime error : NWTPI::NWTPI ** cannot get graphic_display_size.");

		DEBUG ( "NativeWindow", "graphicsGetDisplaySize done : " << deviceWidth << "," << deviceHeight );

		if ( ( windowWidth > deviceWidth ) || ( windowHeight > deviceHeight ) )
			throw runtime_error("runtime error : NativeWindow::NativeWindow ** window sizes exceed device capabilities.");


		displayHandle = displayOpen( LCD );											// set dmDisplay
		updateHandle  = updateStart(0);												// dmUpdate
		elementHandle = elementAdd(updateHandle, displayHandle, windowWidth, windowHeight);	// dmElement will fill the entire window

		DEBUG ( "NativeWindow", " display : " << hex << displayHandle << ", upd : " << hex << updateHandle << ", elm : " << hex << elementHandle << dec );

		updateSync(updateHandle);													// validate our element we have just created.
		windowHandle.element = elementHandle;
		windowHandle.width = windowWidth;
		windowHandle.height = windowHeight;

	} else
		throw runtime_error("runtime error : NativeWindow::NativeWindow ** bcm host already initiated.");
}

NativeWindow::~NativeWindow() {
	if ( bcmInitiated ) {
		displayClose();
		bcm_host_deinit();
	}
}

DISPMANX_DISPLAY_HANDLE_T NativeWindow::displayOpen( unsigned int dev)
{
	DISPMANX_DISPLAY_HANDLE_T dmdh;
	if ( ! displayOpened ) {
		dmdh = vc_dispmanx_display_open( (uint32_t) dev );
		if ( dmdh == DISPMANX_NO_HANDLE )
			throw runtime_error("runtime error : NWTPI::dmDisplayOpen ** does not return an handle");
	}
	displayOpened = true;

	return dmdh;
}

int NativeWindow::displayClose()
{
	return vc_dispmanx_display_close(displayHandle);
}

void NativeWindow::displayGetInfo( DISPMANX_DISPLAY_HANDLE_T disp, DISPMANX_MODEINFO_T info)
{
	vc_dispmanx_display_get_info(disp, &info);
}

DISPMANX_UPDATE_HANDLE_T NativeWindow::updateStart( int prio )
{
	static DISPMANX_UPDATE_HANDLE_T upd;

	upd = vc_dispmanx_update_start( (int32_t) prio);
	if ( upd== DISPMANX_NO_HANDLE )
		throw runtime_error("runtime error : NWTPI::dmUpdateStart ** does not return an handle.");

	return upd;
}

DISPMANX_ELEMENT_HANDLE_T NativeWindow::elementAdd(	DISPMANX_UPDATE_HANDLE_T upd,
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
	if (opaqueness == true) {
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

int NativeWindow::updateSync(DISPMANX_UPDATE_HANDLE_T upd)
{
	return vc_dispmanx_update_submit_sync(upd);
}

#ifdef DEBUG_ON
void NativeWindow::logd(string method, ostream& message) {
	ostringstream& s = dynamic_cast<ostringstream&>(message);
	cout << "==> " << method << " : " << s.str() << endl;
}
#endif
