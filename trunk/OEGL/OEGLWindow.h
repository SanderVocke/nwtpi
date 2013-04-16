#ifndef _OEGL_WINDOW_H
#define _OEGL_WINDOW_H

#include <iostream>
#include <sstream>

#include "EGL/egl.h"
#include "EGL/eglext.h"
#include "GLES2/gl2.h"

#include "NativeWindow.h"
#include "OEGLCapabilities.h"

#ifdef DEBUG_ON
    #define DEBUG( a, b ) logd(a, ostringstream().flush() << b )
#else
    #define DEBUG( a, b )
#endif

using namespace std;

namespace nwtpi {

	class OEGLWindow : protected NativeWindow {

	private:
		string windowTitle;

		OEGLCapabilities* capabilities;

		EGLDisplay 		display;
		EGLConfig 		config;
		EGLint 			configId;
		EGLSurface 		surface;
		//vector <OEGLSurface> surfaces;		// TODO many surfaces / elements associations.
		int  			currentSurfaceId;		// tied to  Native elementId

		EGLContext 		context;
		//EGLClientBuffer 	clientBuffer;	TODO

		EGLContext 		createContext();

		//bool  		egCreateWindow();	TODO
		//bool  		egCreateConfig();	TODO

	#ifdef DEBUG_ON
		void logd(string , ostream& );
	#endif

	public:
		OEGLWindow(string title, unsigned int w, unsigned int h, unsigned char windowAlphaLevel, OEGLCapabilities *);
		~OEGLWindow();

		int  			addSurface();							// default is a WindowSurface
		// int 			addSurface(EOGL_SURFACE_TYPE_ENUM) 		// TODO WINDOW, PBUFFER, PIXMAP

		void			makeCurrentSurface(int surfaceId);

		string 			getWindowTitle();
		unsigned int 	getWindowWidth();
		unsigned int 	getWindowHeight();

		EGLDisplay 		getCurrentDisplay();
		EGLSurface 		getCurrentSurface();

		//	EGLContext*		getCurrentContext();				// TODO

		void 			swapBuffers();

		EGLint getConfigId();

	};
}
#endif
