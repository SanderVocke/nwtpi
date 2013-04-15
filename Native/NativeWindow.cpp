#include "NativeWindow.h"

namespace nwtpi {

bool 		 NativeWindow::bcmInitState = false;
bool 		 NativeWindow::deviceOpenState[] = { false, false, false, false };

NativeWindow::NativeWindow (unsigned int width, unsigned int height, NATIVE_DEVICE_ENUM _device, unsigned char alpha)
	: device(_device), windowWidth(width), windowHeight(height), windowAlpha(alpha), currentElementId(-1)
{
	displayOpen(device);
	createRegion();
}

NativeWindow::~NativeWindow() {
	// delete elements[*]

	if ( displayClose(displayHandle) == 0 )	// closing current device's displayHandle
		deviceOpenState[device] = false;
	else
		throw runtime_error("runtime error : ~NativeWindow ** cannot close device");
}

void NativeWindow::displayOpen(NATIVE_DEVICE_ENUM dev)
{
	if ( ! bcmInitState ) {
		bcm_host_init();
		bcmInitState = true;
	}

	if ( ! deviceOpenState[dev] ) {
		if ( graphics_get_display_size( dev, &deviceWidth, &deviceHeight) < 0)
			throw runtime_error("runtime error : NativeWindow::NativeWindow ** cannot get graphic_display_size.");

		DEBUG ( "NativeWindow", "graphicsGetDisplaySize done : " << deviceWidth << "," << deviceHeight );

		if ( ( windowWidth > deviceWidth ) || ( windowHeight > deviceHeight ) )
			throw runtime_error("runtime error : NativeWindow::NativeWindow ** window sizes exceed device capabilities.");

		displayHandle = vc_dispmanx_display_open( (uint32_t) dev );
		if ( displayHandle == DISPMANX_NO_HANDLE )
			throw runtime_error("runtime error : NativeWindow::displayOpen ** does not return an handle");

		deviceOpenState[dev] = true;
	} else
		throw runtime_error("runtime error : NativeWindow::NativeWindow ** cannot open device twice.");
}

int NativeWindow::displayClose(DISPMANX_DISPLAY_HANDLE_T disp) {

	return vc_dispmanx_display_close( disp );

}

/**
 * 		\fn		void createRegion()
 * 		\brief	create a region by adding a new element that will fill the entire window
 * 		\param	none
 * 		\return void
 */
void NativeWindow::createRegion()
{
	createRegion(0,0,windowWidth,windowHeight,windowAlpha);
}

/**
 *  	\fn		void createRegion(unsigned int x, unsigned int y, unsigned int width, unsigned int height, bool opaqueness)
 *  	\brief	create a marked out region by adding a new element will fill the region
 *  	\param	x Position
 *  	\param 	y Position
 *  	\param	width Size
 *  	\param	height Size
 *  	\param	alpha Alpha level (0x00=no alpha, 0xff full transparent)
 *  	\return	void
 */
void NativeWindow::createRegion(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned char windowAlphaLevel)
{
	NativeElement * elem = new NativeElement(displayHandle, x, y, w, h, windowAlphaLevel );

	elements.push_back(elem);
	currentElementId = elem->getId();

}

/**
 *  	\fn		int createRegionWithImage
 *  	\brief	create a marked out region with from image resource
 *  	\param	uint width, uint height, bool opaqueness, VC_IMAGE_TYPE_T imageType, void * imageData
 *  	\return	int
 */
//int NativeWindow::createRegionWithImage(unsigned int width, unsigned int height, bool opaqueness, VC_IMAGE_TYPE_T imageType, void * imageData)
//{
//	//NativeElement * elem;
//	// resource_create
//
//	// resource_write
//
//	// NativeElement * elem = new element(..)
//	// elements.push_back(elem);
//	// ++currentElement;
//
//
//}

#ifdef DEBUG_ON
void NativeWindow::logd(string method, ostream& message) {
	ostringstream& s = dynamic_cast<ostringstream&>(message);
	cout << "==> " << method << " : " << s.str() << endl;
}
#endif

} // namespace nwtpi
