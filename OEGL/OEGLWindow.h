#ifndef _OEGL_WINDOW_H
#define _OEGL_WINDOW_H

#include <iostream>
#include <sstream>
#include <vector>

#include "EGL/egl.h"
#include "EGL/eglext.h"
#include "GLES2/gl2.h"

#include "NativeWindow.h"
#include "OEGLCapabilities.h"
#include "OEGLSurface.h"

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

		EGLDisplay 			display;
		EGLConfig 			config;
		EGLint 				configId;
		//EGLSurface 			surface;
		vector <OEGLSurface *> surfaces;
		int  				currentSurfaceId;		// tied to  Native elementId

		EGLContext 			context;
		//EGLClientBuffer 	clientBuffer;	TODO

		EGLContext 			createContext();

	#ifdef DEBUG_ON
		void logd(string , ostream& );
	#endif

	public:
		OEGLWindow(string title, unsigned int w, unsigned int h, unsigned char windowAlphaLevel, OEGLCapabilities *);
		~OEGLWindow();

		int  			addWindowSurface(int elementId);		// default to OEGLSurface::WINDOW_SURFACE linked to default element
		// int 			addSurface(OEGL_SURFACE_TYPE_ENUM) 		// TODO WINDOW, PBUFFER, PIXMAP

		void			makeCurrentSurface(int surfaceId);

		string 			getWindowTitle();
		unsigned int 	getWindowWidth();
		unsigned int 	getWindowHeight();

		EGLDisplay 		getDisplay();
		EGLSurface		getCurrentSurfaceHandle();
		int				getCurrentSurfaceId();
		EGLConfig		getConfig();
		EGLint 			getConfigId();
		OEGLSurface * 	getSurfaceById(int surfId);

		//	EGLContext*		getCurrentContext();				// TODO

		void 			swapBuffers();


	};
}
#endif
