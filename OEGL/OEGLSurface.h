#ifndef _OEGL_SURFACE_H
#define _OEGL_SURFACE_H

#include <iostream>
#include <sstream>

#include "EGL/egl.h"
#include "EGL/eglext.h"
#include "GLES2/gl2.h"

#include "NativeWindow.h"
#include "NativeElement.h"

#ifdef DEBUG_ON
    #define DEBUG( a, b ) logd(a, ostringstream().flush() << b )
#else
    #define DEBUG( a, b )
#endif

using namespace std;

namespace nwtpi {

	class OEGLWindow;

	enum OEGL_SURFACE_TYPE_ENUM : unsigned int { OEGL_WINDOW_SURFACE=0, OEGL_PBUFFER_SURFACE, OEGL_PIXMAP_SURFACE };

	class OEGLSurface {

	private:
		OEGL_SURFACE_TYPE_ENUM 	surfaceType;
		OEGLWindow * 			parentWindow;
		NativeElement * 		element;
		EGLSurface				surfaceHandle;

#ifdef DEBUG_ON
	void logd(string , ostream& );
#endif

	protected:
		EGLSurface createSurface();
		//void remove();

		EGLSurface createWindowSurface();
		EGLSurface createPbufferSurface();
		EGLSurface createPixmapSurface();

	public:
		OEGLSurface (OEGL_SURFACE_TYPE_ENUM type, OEGLWindow * p, NativeElement * _element);
		~OEGLSurface();

		EGLSurface 		getHandle();
		int 			getId();			// surfaceId = elementId
		unsigned int 	getx();
		unsigned int 	gety();
		unsigned int 	getWidth();
		unsigned int 	getHeight();
		VC_RECT_T		getRegion();
		OEGL_SURFACE_TYPE_ENUM getType();

	};

} // namespace nwtpi

#endif // _OEGL_SURFACE_H
