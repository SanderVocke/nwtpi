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

		EGLDisplay 		egDisplay;
		EGLConfig 		egConfig;
		EGLint 			egConfigId;
		EGLSurface 		egSurface;
		//vector <EGLSurface> egSurfaces;		// TODO many surfaces / elements associations.
		int  			surfaceId;						// ie NativeElementId
		EGLContext 		egContext;
		//EGLClientBuffer 	clientBuffer;	TODO

		EGLContext 		createContext();
		//bool  		egCreateSurface();	TODO
		//bool  		egCreateWindow();	TODO
		//bool  		egCreateConfig();	TODO

	#ifdef DEBUG_ON
		void logd(string , ostream& );
	#endif

	public:
		// NWTPI (string, unsigned int, unsigned int);	XXX waiting for gcc4.7 ( http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2006/n1986.pdf )
		OEGLWindow(string title, unsigned int w, unsigned int h, unsigned char windowAlphaLevel, OEGLCapabilities *);
		~OEGLWindow();

		string getWindowTitle();
		unsigned int getWindowWidth();
		unsigned int getWindowHeight();

		EGLDisplay getCurrentDisplay();
		EGLSurface getCurrentSurface();
		//	EGLContext*		getCurrentContext();			TODO
		void swapBuffers();

		EGLint getEgConfigId();

	};
}
#endif
