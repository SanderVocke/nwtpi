#include <iostream>
#include <sstream>
#include <stdexcept>

#include "EGLWindow.h"


EGLWindow::EGLWindow (string title, unsigned int w, unsigned int h, bool opacity, EGLCapabilities * caps)
	: NativeWindow(w,h,opacity), windowTitle(title), capabilities(caps)
{

	if ( ( egContext = createContext() ) == EGL_NO_CONTEXT )					// create eglDisplay,Context,Surface from dmWindow.element+w+h
		throw runtime_error("runtime error : EGLWindow::EGLWindow egCreateContext ** failed");

	if ( eglMakeCurrent(egDisplay, egSurface, egSurface, egContext) == EGL_FALSE )
		throw runtime_error("runtime error : EGLWindow::egCreateContext ** eglMakeCurrent context failed.");

	DEBUG ("EGLWindow" , "egCreateContext and make current done." );
}

EGLWindow::~EGLWindow () {
	eglMakeCurrent( egDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
	eglDestroySurface( egDisplay, egSurface );
	eglDestroyContext( egDisplay, egContext );
	eglTerminate( egDisplay );

}

EGLContext EGLWindow::createContext() {
	EGLint 		egMajorVersion;
	EGLint 		egMinorVersion;
	EGLint 		egNumConfigs;

#ifdef DEBUG_ON
	EGLint		egRedSize;
	EGLint		egAlphaSize;
	EGLint		egBufferSize;
#endif

	EGLNativeWindowType eglNativeWindow = &windowHandle;						// void * = &EGL_DISPMANX_WINDOW_T
	EGLint * egAttribList = capabilities->getConfigAttributes();

	DEBUG ("EGLWindow::egCreateContext","attribute[1] as RED_SIZE : " << egAttribList[1]);

	if ( ( egDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY) ) == EGL_NO_DISPLAY )
		throw runtime_error("runtime error : EGLWindow::egCreateContext ** EGL_NO_DISPLAY");

	if ( ! eglInitialize(egDisplay, &egMajorVersion, &egMinorVersion) )
		throw runtime_error("runtime error : EGLWindow::egCreateContext ** eglInitialize failed.");

	if ( eglChooseConfig(egDisplay, egAttribList, &egConfig, 1, &egNumConfigs) == EGL_FALSE )
		throw runtime_error("runtime error : EGLWindow::egCreateContext ** eglChooseConfig failed.");

	egSurface = eglCreateWindowSurface(egDisplay, egConfig, eglNativeWindow, NULL );
	if ( egSurface == EGL_NO_SURFACE )
		throw runtime_error("runtime error : EGLWindow::egCreateContext ** EGL_NO_SURFACE");

	eglGetConfigAttrib(egDisplay,egConfig,EGL_CONFIG_ID,&egConfigID);	// getting our configID

#ifdef DEBUG_ON
	eglGetConfigAttrib(egDisplay,egConfig,EGL_BUFFER_SIZE,&egBufferSize);
	eglGetConfigAttrib(egDisplay,egConfig,EGL_ALPHA_SIZE,&egAlphaSize);
	eglGetConfigAttrib(egDisplay,egConfig,EGL_RED_SIZE,&egRedSize);
#endif

	DEBUG ("NWTPI::egCreateContext","egConfig #" << egConfigID << ", BufferSz : " << egBufferSize << ", AlphaSz : " << egAlphaSize << ", RedSz : " << egRedSize );

	return ( eglCreateContext(egDisplay, egConfig, EGL_NO_CONTEXT, capabilities->getContextAttributes() ) );

}

string EGLWindow::getWindowTitle() {
	return windowTitle;
}

void EGLWindow::swapBuffers() {
	eglSwapBuffers(egDisplay, egSurface);
}

EGLDisplay EGLWindow::getCurrentDisplay() {
	return egDisplay;
}

EGLSurface EGLWindow::getCurrentSurface() {
	return egSurface;
}

EGLint 	EGLWindow::getEgConfigID() {
	return egConfigID;
}
#ifdef DEBUG_ON
void EGLWindow::logd(string method, ostream& message) {
	ostringstream& s = dynamic_cast<ostringstream&>(message);
	cout << "==> " << method << " : " << s.str() << endl;
}
#endif
