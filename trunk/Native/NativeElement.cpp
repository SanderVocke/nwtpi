#include "NativeElement.h"

namespace nwtpi {

/**
 * 	NativeElement
 */

const unsigned int NativeElement::defaultPriority = 1;		// update priority
int NativeElement::elementsCounter = -1;

NativeElement::NativeElement(DISPMANX_DISPLAY_HANDLE_T display, unsigned int x, unsigned y, unsigned int w, unsigned int h, unsigned char _alpha)
	: elementId(-1), displayHandle(display), priority(defaultPriority), updateLockState(false), eglSurface(EGL_NO_SURFACE)
{
	setRegion(x,y,w,h);
	setAlpha(_alpha);

	// void resource creation :
	resource = new NativeResource(region);
	updateHandle = updateStart(priority);	// this seem to start a thread and return a new handle
	elementHandle = add(displayHandle, updateHandle, region, resource->getRegion(), alpha);	// updateHandle => attach element to thread ?
	elementId = elementsCounter += 1;		// TODO - collection

	//updateSync(updateHandle);

	eglNativeWindowHandle.element = elementHandle;
	eglNativeWindowHandle.width   = region.width;
	eglNativeWindowHandle.height  = region.height;
}

DISPMANX_UPDATE_HANDLE_T NativeElement::updateStart(int prio)
{
	DISPMANX_UPDATE_HANDLE_T upd = (uint32_t) 0;

	upd = vc_dispmanx_update_start( (int32_t) prio);
	if ( upd== DISPMANX_NO_HANDLE )
		throw runtime_error("runtime error : NativeElement::updateStart ** does not return an handle.");

	return upd;

}

DISPMANX_ELEMENT_HANDLE_T NativeElement::add(DISPMANX_DISPLAY_HANDLE_T disp, DISPMANX_UPDATE_HANDLE_T upd,
											 VC_RECT_T regionDst, VC_RECT_T regionSrc,
											 VC_DISPMANX_ALPHA_T _alpha)
{
	return vc_dispmanx_element_add (upd,						/* update 						*/
									disp,						/* display 						*/
									0,							/* int32_t layer				*/
									&regionDst,					/* dest_rect 					*/
									0,							/* src							*/
									&regionSrc,					/* src_rect 					*/
									DISPMANX_PROTECTION_NONE,	/* DISPMANX_PROTECTION 		TODO */
									&alpha, 					/* VC_DISPMANX_ALPHA_T alpha     */
									0,							/* DISPMANX_CLAMP_T 		TODO */
									DISPMANX_NO_ROTATE);		/* DISPMANX_TRANSFORM_T 	TODO */
}

int NativeElement::updateSync()
{
	return updateSync(updateHandle);
}

int NativeElement::updateSync(DISPMANX_UPDATE_HANDLE_T upd)
{
	return vc_dispmanx_update_submit_sync(upd);
}

EGLNativeWindowType NativeElement::getEglNativeWindowType()
{
	return (EGLNativeWindowType) &eglNativeWindowHandle;
}

int NativeElement::getId()
{
	return elementId;
}

void NativeElement::setRegion(unsigned int x, unsigned y, unsigned int w, unsigned int h)
{
	region.x 	  = (uint32_t) x;
	region.y 	  = (uint32_t) y;
	region.width  = (uint32_t) w;
	region.height = (uint32_t) h;

}

void NativeElement::setUpdateLockState(bool lockState)
{
	updateLockState = lockState;
}

void NativeElement::setAlpha(unsigned char _alpha)
{
	memset(&alpha, 0x0, sizeof(VC_DISPMANX_ALPHA_T));

	if (_alpha == 0) {
			alpha.flags = DISPMANX_FLAGS_ALPHA_FIXED_ALL_PIXELS;
			alpha.opacity = 0xFF;
			alpha.mask = 0;
	} else {
			alpha.flags = DISPMANX_FLAGS_ALPHA_FROM_SOURCE;
			alpha.opacity = 0xFF;
			alpha.mask = _alpha;
	}
}

VC_DISPMANX_ALPHA_T NativeElement::getAlpha()
{
	return alpha;
}

VC_RECT_T NativeElement::getRegion()
{
	return region;
}

#ifdef DEBUG_ON
void NativeElement::logd(string method, ostream& message) {
	ostringstream& s = dynamic_cast<ostringstream&>(message);
	cout << "==> " << method << " : " << s.str() << endl;
}
#endif

}	// namespace nwtpi
