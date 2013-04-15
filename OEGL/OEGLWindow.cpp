#include <iostream>
#include <sstream>
#include <stdexcept>

#include "OEGLWindow.h"

namespace nwtpi {


OEGLWindow::OEGLWindow (string title, unsigned int w, unsigned int h, unsigned char alpha, OEGLCapabilities * caps)
	: NativeWindow(w, h, NATIVE_DEVICE_ENUM::LCD, alpha), windowTitle(title), capabilities(caps), surfaceId(-1)
{

	if ( ( egContext = createContext() ) == EGL_NO_CONTEXT )					// create eglDisplay,Context,Surface from dmWindow.element+w+h
		throw runtime_error("runtime error : OEGLWindow::OEGLWindow egCreateContext ** failed");

	if ( eglMakeCurrent(egDisplay, egSurface, egSurface, egContext) == EGL_FALSE )
		throw runtime_error("runtime error : OEGLWindow::egCreateContext ** eglMakeCurrent context failed.");

	DEBUG ("OEGLWindow" , "egCreateContext and make current done." );
}

OEGLWindow::~OEGLWindow () {
	eglMakeCurrent( egDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
	eglDestroySurface( egDisplay, egSurface );
	eglDestroyContext( egDisplay, egContext );
	eglTerminate( egDisplay );

}

EGLContext OEGLWindow::createContext() {
	EGLint 		egMajorVersion;
	EGLint 		egMinorVersion;
	EGLint 		egNumConfigs;

#ifdef DEBUG_ON
	EGLint		egRedSize;
	EGLint		egAlphaSize;
	EGLint		egBufferSize;
#endif

	//EGLNativeWindowType eglNativeWindow = &windowHandle;						// void * = &EGL_DISPMANX_WINDOW_T
	//EGLNativeWindowType eglNativeWindow = elements[currentElementId]->getEglNativeWindowType();
	EGLint * egAttribList = capabilities->getConfigAttributes();

	DEBUG ("OEGLWindow::egCreateContext","attribute[1] as RED_SIZE : " << egAttribList[1]);

	if ( ( egDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY) ) == EGL_NO_DISPLAY )
		throw runtime_error("runtime error : OEGLWindow::egCreateContext ** EGL_NO_DISPLAY");

	if ( ! eglInitialize(egDisplay, &egMajorVersion, &egMinorVersion) )
		throw runtime_error("runtime error : OEGLWindow::egCreateContext ** eglInitialize failed.");

	if ( eglChooseConfig(egDisplay, egAttribList, &egConfig, 1, &egNumConfigs) == EGL_FALSE )
		throw runtime_error("runtime error : OEGLWindow::egCreateContext ** eglChooseConfig failed.");

	egSurface = eglCreateWindowSurface(egDisplay, egConfig, elements[currentElementId]->getEglNativeWindowType(), NULL );
	if ( egSurface == EGL_NO_SURFACE )
		throw runtime_error("runtime error : OEGLWindow::egCreateContext ** EGL_NO_SURFACE");

	surfaceId = currentElementId;	// we link surface<->NativeElement together

	eglGetConfigAttrib(egDisplay,egConfig,EGL_CONFIG_ID,&egConfigId);	// getting our configId

#ifdef DEBUG_ON
	eglGetConfigAttrib(egDisplay,egConfig,EGL_BUFFER_SIZE,&egBufferSize);
	eglGetConfigAttrib(egDisplay,egConfig,EGL_ALPHA_SIZE,&egAlphaSize);
	eglGetConfigAttrib(egDisplay,egConfig,EGL_RED_SIZE,&egRedSize);
#endif

	DEBUG ("NWTPI::egCreateContext","egConfig #" << egConfigId << ", BufferSz : " << egBufferSize << ", AlphaSz : " << egAlphaSize << ", RedSz : " << egRedSize );

	return ( eglCreateContext(egDisplay, egConfig, EGL_NO_CONTEXT, capabilities->getContextAttributes() ) );

}

string OEGLWindow::getWindowTitle() {
	return windowTitle;
}

void OEGLWindow::swapBuffers() {
	eglSwapBuffers(egDisplay, egSurface);
}

EGLDisplay OEGLWindow::getCurrentDisplay() {
	return egDisplay;
}

EGLSurface OEGLWindow::getCurrentSurface() {
	return egSurface;
}

EGLint 	OEGLWindow::getEgConfigId() {
	return egConfigId;
}
#ifdef DEBUG_ON
void OEGLWindow::logd(string method, ostream& message) {
	ostringstream& s = dynamic_cast<ostringstream&>(message);
	cout << "==> " << method << " : " << s.str() << endl;
}
#endif

} // namespace nwtpi
