/*
 * EGLCapabilities.h
 *
 *  Created on: Apr 3, 2013
 *      Author: valery
 */

#ifndef _EGLCAPABILITIES_H
#define _EGLCAPABILITIES_H

#include <iostream>
#include <sstream>
#include <vector>

#include "bcm_host.h"

#include "EGL/egl.h"
#include "EGL/eglext.h"
#include "GLES2/gl2.h"

#ifdef DEBUG_ON
    #define DEBUG( a, b ) logd(a, ostringstream().flush() << b )
#else
    #define DEBUG( a, b )
#endif


using namespace std;

class EGLCapabilities {
private:
	unsigned int  	bufferSize;
	unsigned int    redSize;
	unsigned int    greenSize;
	unsigned int    blueSize;
	unsigned int    alphaSize;

	unsigned int    depthSize;
	unsigned int	stencilSize;

	// Support for transparent windows containing OpenGL content
	bool 	backgroundOpaque;
	int     transparentValueRed;
	int     transparentValueGreen;
	int     transparentValueBlue;
	int     transparentValueAlpha;

	// Switch for on- or offscreen
	bool onscreen;

	// offscreen bitmap mode
	bool isBitmap;

	vector <EGLint> attributes;

	static const EGLint RGB565_SET[];
	static const EGLint RGB888_SET[];
	static const EGLint RGBA888_SET[];

	static const EGLint * rgbProfiles[];

#ifdef DEBUG_ON
	void logd(string , ostream& );
#endif

public:
	enum RGB_CAPS_ENUM : unsigned int { RGB565=0, RGB888, RGBA888 };

	EGLCapabilities(RGB_CAPS_ENUM);
	~EGLCapabilities();

	EGLint* getAttributes();
//	EGLint* getEglConfigAttrib(unsigned int, EGLenum);

};

#endif /* _EGLCAPABILITIES_H */
