#include "NativeResource.h"

namespace nwtpi
{

NativeResource::NativeResource(VC_RECT_T _region)													// no resource handle, simple void region (EGL)
	: resourceHandle((uint32_t)0), rgbType((VC_IMAGE_TYPE_T) 0), nativeImageHandle((uint32_t)0), imageData( (void *) 0), imagePitch(0)
{
	setRegion(_region.x,_region.y,_region.width,_region.height);
}

NativeResource::NativeResource(unsigned int x, unsigned int y, unsigned int w, unsigned int h)		// no resource handle, simple void region (EGL)
	: resourceHandle((uint32_t)0), rgbType((VC_IMAGE_TYPE_T) 0), nativeImageHandle((uint32_t)0), imageData( (void *) 0), imagePitch(0)
{
	setRegion(x,y,w,h);
}

NativeResource::NativeResource(unsigned int x, unsigned int y, unsigned int w, unsigned int h, VC_IMAGE_TYPE_T type, void * _imageData, int _imagePitch)
	: resourceHandle(0), rgbType(type), nativeImageHandle(0), imageData(_imageData ), imagePitch(_imagePitch)
{
	int ret;

	setRegion(x,y,w,h);

    resourceHandle = vc_dispmanx_resource_create( rgbType, region.width, region.height, &nativeImageHandle );
    ret = vc_dispmanx_resource_write_data( resourceHandle, rgbType, imagePitch, imageData, &region );
    if ( ret != 0 )
    	throw runtime_error("runtime error : NativeResource::NativeResource ** resourceWriteData failed.");

}

NativeResource::~NativeResource()
{

}

void NativeResource::setRegion(unsigned int x, unsigned y, unsigned int w, unsigned int h)
{
	region.x 	  = (uint32_t) x;
	region.y 	  = (uint32_t) y;
	region.width  = (uint32_t) w << 16;
	region.height = (uint32_t) h << 16;

}

VC_RECT_T NativeResource::getRegion()
{
	return region;
}

bool NativeResource::hasHandle()
{
	return ( (resourceHandle == (uint32_t) 0) ? false : true );
}


}	// namespace nwtpi
