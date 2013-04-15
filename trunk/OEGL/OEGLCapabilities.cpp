#include <iostream>
#include <sstream>
#include <stdexcept>

#include "OEGLCapabilities.h"

namespace nwtpi {

// Static data initialization
const EGLint OEGLCapabilities::RGB565_SET[] =  { EGL_BUFFER_SIZE, 16 };
const EGLint OEGLCapabilities::RGB888_SET[] =  { EGL_BUFFER_SIZE, 24, EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8 };
const EGLint OEGLCapabilities::RGBA888_SET[] = { EGL_BUFFER_SIZE, 32, EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_ALPHA_SIZE, 8 };

const EGLint * OEGLCapabilities::rgbProfiles[] = {
			(EGLint *) RGB565_SET,
			(EGLint *) RGB888_SET,
			(EGLint *) RGBA888_SET
};

const unsigned int OEGLCapabilities::rgbsetSize[] = {
		sizeof(OEGLCapabilities::RGB565_SET) / sizeof(EGLint),
		sizeof(OEGLCapabilities::RGB888_SET) / sizeof(EGLint),
		sizeof(OEGLCapabilities::RGBA888_SET) / sizeof(EGLint)
};

// Pi context => GLES/2 :                                             v               v
const EGLint OEGLCapabilities::raspberryPiEglContext[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };



OEGLCapabilities::OEGLCapabilities(NWTPI_RGB_CAPS_ENUM choosenProfile) {

	attributes.insert(attributes.end(), &rgbProfiles[choosenProfile][0], &rgbProfiles[choosenProfile][rgbsetSize[choosenProfile]]);

	attributes.insert(attributes.end(), EGL_NONE);
}

OEGLCapabilities::~OEGLCapabilities() { }

EGLint * OEGLCapabilities::getConfigAttributes() {
	return &attributes[0];
}

EGLint * OEGLCapabilities::getContextAttributes() {
	return (EGLint *) &raspberryPiEglContext;
}

// setAttributes don't verify EGL conformant parameters XXX
void OEGLCapabilities::setAttribute(EGLenum egEnum, EGLint value) {
	attributes.pop_back();

	attributes.push_back(egEnum);
	attributes.push_back(value);
	attributes.push_back(EGL_NONE);
}
/**
 *  \fn		ELGint * getEglConfigAttrib(unsigned int eglConfignumber, EGLenum eglType)
 *  \brief	return eglGetConfigAttrib attribute value.
 * 	\param 	eglConfigNumber EGL config number as previously got from eglGetConfigs.
 * 	\param	eglType EGL type to get.
 * 	\return	pointer to value got from eglGetConfigAttrib(,,,&value);
 *
 *  (doxygen template test)
 */
//EGLint * OEGLCapabilities::getEglConfigAttrib(EGLConfig eglConfig, EGLenum eglType) {
//	EGLDisplay display;
//	EGLint numConfigs;
//	static EGLint value;
//	int rc;
//
//	if ( ( display = eglGetDisplay(EGL_DEFAULT_DISPLAY) ) == EGL_NO_DISPLAY )
//		return EGL_FALSE;
//
//	// eglInit is expected to be already called, but EGL specs specify we can call it again w/o side effect
//	if ( ( rc = eglInitialize(display, NULL, NULL) ) == EGL_FALSE )
//		return EGL_FALSE;
//
//	if ( ( rc = eglGetConfigs(display, NULL, 0, &numConfigs) ) == EGL_FALSE )
//		return EGL_FALSE;
//
//	EGLConfig *configs = new EGLConfig[numConfigs];
//
//	if ( ( rc = eglGetConfigs(display,configs,numConfigs,&numConfigs) ) == EGL_FALSE )
//		return EGL_FALSE;
//
//	eglGetConfigAttrib(display,configs[eglConfigNumber],eglType,&value);
//
//	return &value;
//}

#ifdef DEBUG_ON
void OEGLCapabilities::logd(string method, ostream& message) {
	ostringstream& s = dynamic_cast<ostringstream&>(message);
	cout << "==> " << method << " : " << s.str() << endl;
}
#endif

} // namespace nwtpi
