#include <iostream>
#include <sstream>
#include <stdexcept>

#include "EGLCapabilities.h"

// Static data initialization
const EGLint EGLCapabilities::RGB565_SET[] =  { EGL_BUFFER_SIZE, 16 };
const EGLint EGLCapabilities::RGB888_SET[] =  { EGL_BUFFER_SIZE, 24, EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8 };
const EGLint EGLCapabilities::RGBA888_SET[] = { EGL_BUFFER_SIZE, 32, EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_ALPHA_SIZE, 8 };

const EGLint * EGLCapabilities::rgbProfiles[] = {
			(EGLint *) RGB565_SET,
			(EGLint *) RGB888_SET,
			(EGLint *) RGBA888_SET
};

const unsigned int EGLCapabilities::rgbsetSize[] = {
		sizeof(EGLCapabilities::RGB565_SET) / sizeof(EGLint),
		sizeof(EGLCapabilities::RGB888_SET) / sizeof(EGLint),
		sizeof(EGLCapabilities::RGBA888_SET) / sizeof(EGLint)
};

// Pi context => GLES/2 :                                             v               v
const EGLint EGLCapabilities::raspberryPiEglContext[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };



EGLCapabilities::EGLCapabilities(RGB_CAPS_ENUM choosenProfile) {

	attributes.insert(attributes.end(), &rgbProfiles[choosenProfile][0], &rgbProfiles[choosenProfile][rgbsetSize[choosenProfile]]);

	attributes.insert(attributes.end(), EGL_NONE);
}

EGLCapabilities::~EGLCapabilities() { }

EGLint * EGLCapabilities::getConfigAttributes() {
	return &attributes[0];
}

EGLint * EGLCapabilities::getContextAttributes() {
	return (EGLint *) &raspberryPiEglContext;
}

// setAttributes don't verify EGL conformant parameters XXX
void EGLCapabilities::setAttribute(EGLenum egEnum, EGLint value) {
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
//EGLint * EGLCapabilities::getEglConfigAttrib(EGLConfig eglConfig, EGLenum eglType) {
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
void EGLCapabilities::logd(string method, ostream& message) {
	ostringstream& s = dynamic_cast<ostringstream&>(message);
	cout << "==> " << method << " : " << s.str() << endl;
}
#endif
