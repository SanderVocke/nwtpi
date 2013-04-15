#ifndef _NATIVE_RESOURCE_H
#define _NATIVE_RESOURCE_H

#include <iostream>
#include <sstream>
#include <vector>
#include <stdexcept>

#include "bcm_host.h"


#ifdef DEBUG_ON
    #define DEBUG( a, b ) logd(a, ostringstream().flush() << b )
#else
    #define DEBUG( a, b )
#endif

using namespace std;

namespace nwtpi {

class NativeResource
{
	protected:
		DISPMANX_RESOURCE_HANDLE_T	resourceHandle;
		VC_RECT_T       			region;						// x, y, w<<16, h<<16	src_rect
		VC_IMAGE_TYPE_T 			rgbType;					//
		uint32_t                    nativeImageHandle;			// ie vc_image_ptr;
		void * 						imageData;
		int 						imagePitch;

		DISPMANX_RESOURCE_HANDLE_T	create(VC_IMAGE_TYPE_T _imageType, uint32_t width, uint32_t height, uint32_t *_imageHandle );
		int 						writeData(DISPMANX_RESOURCE_HANDLE_T res, VC_IMAGE_TYPE_T src_type, int src_pitch, void * src_address, const VC_RECT_T * rect );
		int							remove(DISPMANX_RESOURCE_HANDLE_T);

		void 						setRegion(unsigned int x, unsigned y, unsigned int w, unsigned int h);
#ifdef DEBUG_ON
		void logd(string , ostream& );
#endif


	public:
		NativeResource(VC_RECT_T _region);
		NativeResource(unsigned int x, unsigned int y, unsigned int w, unsigned int h);		// no resource handle, ie 0 (EGL)
		NativeResource(unsigned int x, unsigned int y, unsigned int w, unsigned int h, VC_IMAGE_TYPE_T type, void * imageData, int imagePitch);
		~NativeResource();

		VC_RECT_T 					getRegion();
		DISPMANX_RESOURCE_HANDLE_T	getHandle();
		bool						hasHandle();

};

} 		// namespace nwtpi

#endif 	// _NATIVE_RESOURCE_H
