#include "OEGLSurface.h"
#include "OEGLWindow.h"

namespace nwtpi {

OEGLSurface::OEGLSurface(OEGL_SURFACE_TYPE_ENUM type, OEGLWindow * p, NativeElement * _element)
	: surfaceType(type), parentWindow(p), element(_element)
{
	surfaceHandle = createSurface();

	if ( surfaceHandle == EGL_NO_SURFACE )
		throw runtime_error("runtime error : OEGLSurface::OEGLSurface ** EGL_NO_SURFACE");

}

OEGLSurface::~OEGLSurface ()
{

}

EGLSurface OEGLSurface::createSurface() {

	switch (surfaceType)
	{
		case OEGL_WINDOW_SURFACE:  return ( createWindowSurface() ); break;
		case OEGL_PBUFFER_SURFACE: return ( createPbufferSurface() ); break;
		case OEGL_PIXMAP_SURFACE:  return ( createPixmapSurface() ); break;
		default: return ( EGL_NO_SURFACE );
	}

	return ( EGL_NO_SURFACE );
}

EGLSurface OEGLSurface::createWindowSurface()
{
	return ( eglCreateWindowSurface(parentWindow->getDisplay(), parentWindow->getConfig(), element->getEglNativeWindowType(), NULL ) );
}

EGLSurface OEGLSurface::createPbufferSurface()
{
	EGLSurface handle = EGL_NO_SURFACE;

	return ( handle );
}

EGLSurface OEGLSurface::createPixmapSurface()
{
	EGLSurface handle = EGL_NO_SURFACE;

	return ( handle );
}


EGLSurface OEGLSurface::getHandle()
{
	return surfaceHandle;
}

int OEGLSurface::getId()			// surfaceId = elementId
{
	return ( element->getId() );
}

unsigned int OEGLSurface::getx()
{
	return ( (unsigned int) element->getRegion().x );
}

unsigned int OEGLSurface::gety()
{
	return ( (unsigned int) element->getRegion().y );
}

unsigned int OEGLSurface::getWidth()
{
	return ( (unsigned int) element->getRegion().width );
}

unsigned int OEGLSurface::getHeight()
{
	return ( (unsigned int) element->getRegion().height );
}

VC_RECT_T OEGLSurface::getRegion()
{
	return ( element->getRegion() );
}

OEGL_SURFACE_TYPE_ENUM OEGLSurface::getType()
{
	return ( surfaceType );
}

#ifdef DEBUG_ON
void OEGLSurface::logd(string method, ostream& message) {
	ostringstream& s = dynamic_cast<ostringstream&>(message);
	cout << "==> " << method << " : " << s.str() << endl;
}
#endif

} // namespace nwtpi
