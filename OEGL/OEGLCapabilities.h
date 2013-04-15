/*
 * OEGLCapabilities.h
 *
 *  Created on: Apr 3, 2013
 *      Author: valery
 */

#ifndef _OEGL_CAPABILITIES_H
#define _OEGL_CAPABILITIES_H

#include <iostream>
#include <sstream>
#include <vector>

#include "bcm_host.h"

#include "EGL/egl.h"
#include "EGL/eglext.h"
#include "GLES2/gl2.h"

#include "NWTPICommon.h"

#ifdef DEBUG_ON
    #define DEBUG( a, b ) logd(a, ostringstream().flush() << b )
#else
    #define DEBUG( a, b )
#endif

using namespace std;

namespace nwtpi {

	class OEGLCapabilities {
	private:
		vector <EGLint> attributes;

		static const EGLint RGB565_SET[];
		static const EGLint RGB888_SET[];
		static const EGLint RGBA888_SET[];

		static const unsigned int rgbsetSize[];

		static const EGLint * rgbProfiles[];

		static const EGLint raspberryPiEglContext[];

	#ifdef DEBUG_ON
		void logd(string , ostream& );
	#endif

	public:
		enum NWTPI_RGB_CAPS_ENUM : unsigned int { RGB565=0, RGB888, RGBA888, RGB_CAPS_ENUM_COUNT };

		OEGLCapabilities(NWTPI_RGB_CAPS_ENUM);
		~OEGLCapabilities();

		EGLint* getConfigAttributes();
		EGLint* getContextAttributes();

		void setAttribute(EGLenum, EGLint);

	};
}
#endif /* _OEGL_CAPABILITIES_H */
