#include <iostream>
#include <sstream>
#include <stdexcept>

#include "OEGLWindow.h"

namespace nwtpi {


OEGLWindow::OEGLWindow (string title, unsigned int w, unsigned int h, unsigned char alpha, OEGLCapabilities * caps)
	: NativeWindow(w, h, NATIVE_DEVICE_ENUM::LCD, alpha), windowTitle(title), capabilities(caps), currentSurfaceId(-1)
{

	if ( ( context = createContext() ) == EGL_NO_CONTEXT )					// create eglDisplay,Context,Surface from dmWindow.element+w+h
		throw runtime_error("runtime error : OEGLWindow::OEGLWindow egCreateContext ** failed");

	// NativeWindow already has a default element, so we just link our surface to it.
	makeCurrentSurface(addWindowSurface(currentElementId));

	// 1st method
//	DEBUG ("OEGLWindow",
//			"currentElementId : " << currentElementId <<
//			", currentSurfaceId : " << currentSurfaceId <<
//			", surfaceHandle : " << hex << surface << dec );
//
//	if ( eglMakeCurrent(display, surface, surface, context) == EGL_FALSE )
//		throw runtime_error("runtime error : OEGLWindow::egCreateContext ** eglMakeCurrent context failed.");

//	 2cd method
	DEBUG ("OEGLWindow",
				"currentElementId : " << currentElementId <<
				", currentSurfaceId : " << currentSurfaceId <<
				", surfaceHandle : " << hex << surfaces[currentSurfaceId]->getHandle() << dec );

	if ( eglMakeCurrent(display, surfaces[currentSurfaceId]->getHandle(), surfaces[currentSurfaceId]->getHandle(), context) == EGL_FALSE )
		throw runtime_error("runtime error : OEGLWindow::egCreateContext ** eglMakeCurrent context failed.");

	DEBUG ("OEGLWindow" , "egCreateContext and make current done." );
}

OEGLWindow::~OEGLWindow () {
	eglMakeCurrent( display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
	//eglDestroySurface( display, surface );
	eglDestroySurface( display, surfaces[currentSurfaceId]->getHandle() );	// TODO bulk delete surfaces[*]
	eglDestroyContext( display, context );
	eglTerminate( display );

}

EGLContext OEGLWindow::createContext() {
	EGLint 		eglMajorVersion;
	EGLint 		eglMinorVersion;
	EGLint 		eglNumConfigs;

#ifdef DEBUG_ON
	EGLint		eglRedSize;
	EGLint		eglAlphaSize;
	EGLint		eglBufferSize;
#endif

	EGLint * eglAttribList = capabilities->getConfigAttributes();

	DEBUG ("OEGLWindow::egCreateContext","attribute[1] as RED_SIZE : " << eglAttribList[1]);

	if ( ( display = eglGetDisplay(EGL_DEFAULT_DISPLAY) ) == EGL_NO_DISPLAY )
		throw runtime_error("runtime error : OEGLWindow::egCreateContext ** EGL_NO_DISPLAY");

	if ( ! eglInitialize(display, &eglMajorVersion, &eglMinorVersion) )
		throw runtime_error("runtime error : OEGLWindow::egCreateContext ** eglInitialize failed.");

	if ( eglChooseConfig(display, eglAttribList, &config, 1, &eglNumConfigs) == EGL_FALSE )
		throw runtime_error("runtime error : OEGLWindow::egCreateContext ** eglChooseConfig failed.");

	eglGetConfigAttrib(display,config,EGL_CONFIG_ID,&configId);	// getting our configId

#ifdef DEBUG_ON
	eglGetConfigAttrib(display,config,EGL_BUFFER_SIZE,&eglBufferSize);
	eglGetConfigAttrib(display,config,EGL_ALPHA_SIZE,&eglAlphaSize);
	eglGetConfigAttrib(display,config,EGL_RED_SIZE,&eglRedSize);
#endif

	DEBUG ("OEGLWindow::egCreateContext","egConfig #" << configId << ", BufferSz : " << eglBufferSize << ", AlphaSz : " << eglAlphaSize << ", RedSz : " << eglRedSize );

	return ( eglCreateContext(display, config, EGL_NO_CONTEXT, capabilities->getContextAttributes() ) );

}

/**
 * 		\fn		int addWindowSurface
 * 		\brief	add a window surface from element 'elementId'
 * 		\param	elementId : element id to linked
 *		\return	surfaceId
 */
int OEGLWindow::addWindowSurface(int elemId)
{
	// 1st method
//	surface = eglCreateWindowSurface(display, config, elements[elemId]->getEglNativeWindowType(), NULL );
//	if ( surface == EGL_NO_SURFACE )
//		throw runtime_error("runtime error : OEGLWindow::egCreateContext ** EGL_NO_SURFACE");
//
//	return ( elemId );

	// 2cd method
	OEGLSurface * surf = new OEGLSurface(OEGL_WINDOW_SURFACE, this, elements[elemId]);

	if ( surf->getHandle() == EGL_NO_SURFACE )
		throw runtime_error("runtime error : OEGLWindow::addWindowSurface ** EGL_NO_SURFACE");

	surfaces.push_back(surf);

	return ( surf->getId() );
}

/**
 * 		\fn		void makeCurrentSurface
 * 		\brief	set currentSurfaceId from a surfaceId
 * 		\param	int surfaceId
 * 		\return	void
 */
void OEGLWindow::makeCurrentSurface(int id)
{
	currentSurfaceId = id;
}

string OEGLWindow::getWindowTitle() {
	return windowTitle;
}

void OEGLWindow::swapBuffers() {
	// 1st method
	//eglSwapBuffers(display, surface);

	// 2cd method
	eglSwapBuffers(display, surfaces[currentSurfaceId]->getHandle() );
}

EGLDisplay OEGLWindow::getDisplay() {
	return display;
}

EGLSurface OEGLWindow::getCurrentSurfaceHandle() {
	//return surface;
	return ( surfaces[currentSurfaceId]->getHandle() );
}

int OEGLWindow::getCurrentSurfaceId()
{
	return currentSurfaceId;
}

EGLConfig OEGLWindow::getConfig()
{
	return config;
}

EGLint 	OEGLWindow::getConfigId() {
	return configId;
}

#ifdef DEBUG_ON
void OEGLWindow::logd(string method, ostream& message) {
	ostringstream& s = dynamic_cast<ostringstream&>(message);
	cout << "==> " << method << " : " << s.str() << endl;
}
#endif

} // namespace nwtpi
