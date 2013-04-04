#include <iostream>
#include <sstream>
#include <stdexcept>

#include "EGLCapabilities.h"

// Static data initialization
const EGLint EGLCapabilities::RGB565_SET[] =  { EGL_RED_SIZE, 5, EGL_GREEN_SIZE, 6, EGL_BLUE_SIZE, 5, EGL_ALPHA_SIZE, 0 };
const EGLint EGLCapabilities::RGBA565_SET[] = { EGL_RED_SIZE, 5, EGL_GREEN_SIZE, 6, EGL_BLUE_SIZE, 5, EGL_ALPHA_SIZE, 8 };
const EGLint EGLCapabilities::RGB888_SET[] =  { EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_ALPHA_SIZE, 0 };
const EGLint EGLCapabilities::RGBA888_SET[] = { EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_ALPHA_SIZE, 8 };

const EGLint * EGLCapabilities::rgbProfiles[] = {
			(EGLint *) RGB565_SET,
			(EGLint *) RGBA565_SET,
			(EGLint *) RGB888_SET,
			(EGLint *) RGBA888_SET
};

//EGLint EGLCapabilities::attributes[] = {
//		EGL_BUFFER_SIZE,	24,
//		EGL_RED_SIZE,       5,
//		EGL_GREEN_SIZE,     6,
//		EGL_BLUE_SIZE,      5,
//		EGL_ALPHA_SIZE,     0,
//		EGL_DEPTH_SIZE,     0,
//		EGL_STENCIL_SIZE,   0,
//		EGL_SAMPLE_BUFFERS, 0,
//		EGL_NONE
//};

EGLCapabilities::EGLCapabilities(RGB_CAPS_ENUM choosenProfile) {

	unsigned int rgbsetSize = sizeof(RGB565_SET) / sizeof(EGLint);

	attributes.insert(attributes.end(), &rgbProfiles[choosenProfile][0], &rgbProfiles[choosenProfile][rgbsetSize]);

	attributes.insert(attributes.end(), EGL_NONE);

}

EGLCapabilities::~EGLCapabilities() { }

EGLint * EGLCapabilities::getAttributes() {
	return &attributes[0];
}

#ifdef DEBUG_ON
void EGLCapabilities::logd(string method, ostream& message) {
	ostringstream& s = dynamic_cast<ostringstream&>(message);
	cout << "==> " << method << " : " << s.str() << endl;
}
#endif
