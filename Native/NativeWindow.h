#ifndef _NATIVE_WINDOW_H
#define _NATIVE_WINDOW_H

#include <iostream>
#include <sstream>
#include <vector>

#include "bcm_host.h"
#include "NativeElement.h"

#ifdef DEBUG_ON
    #define DEBUG( a, b ) logd(a, ostringstream().flush() << b )
#else
    #define DEBUG( a, b )
#endif

using namespace std;

namespace nwtpi {

	enum NATIVE_DEVICE_ENUM : unsigned int { LCD=0, AUX_LCD, HDMI, SDTV, NATIVE_DEVICE_ENUM_COUNT };

class NativeWindow {

	private:
		// TODO will be better to put this on a class like NativeDevice / only one displayHandle by device
		static bool					bcmInitState;
		static bool					deviceOpenState[];

	protected:
		NATIVE_DEVICE_ENUM			device;
	    unsigned int				deviceWidth;
	    unsigned int		 		deviceHeight;
		unsigned int 				windowWidth;
		unsigned int 				windowHeight;
		DISPMANX_DISPLAY_HANDLE_T   displayHandle;
		unsigned char				windowAlpha;
		vector <NativeElement *>	elements;
		int 						currentElementId;
		int							elementSyncLockId;

		void 						displayOpen(NATIVE_DEVICE_ENUM dev);
		int							displayClose(DISPMANX_DISPLAY_HANDLE_T);

#ifdef DEBUG_ON
		void logd(string , ostream& );
#endif

	public:

		NativeWindow(unsigned int w, unsigned int h, NATIVE_DEVICE_ENUM dev, unsigned char alpha);
		~NativeWindow();

		int 			addElement();
		int 			addElement(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char alpha);

		void 			makeCurrentElement(int elemId);
		bool			syncElement(int element);
		int 			getLastElementId();
		NativeElement * getCurrentElement();
		int 			getCurrentElementId();
		NativeElement * findElementById(int elemId);

	 	// marked out region + image resource TODO createElementImage
		//int 	addImageElement(unsigned int width, unsigned int height, bool opaqueness, VC_IMAGE_TYPE_T imageType, void * imageData);

		// void drawImage();
		// void drawScene();

	};

} 	// namespace nwtpi

#endif 	// _NATIVE_WINDOW_H
