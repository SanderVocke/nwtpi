#include <iostream>
#include <cassert>
#include "EGL/egl.h"
#include "EGL/eglext.h"


int main()
{
	EGLDisplay display;
	display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	assert(display !=EGL_NO_DISPLAY);
	int major;
	int minor;
	EGLBoolean result;

	result = eglInitialize(display, &major, &minor);
	assert(result != EGL_FALSE );
	std::cout<<"Major version "<<major<<" minor "<<minor<<"\n";

	EGLint numConfigs;
	result=eglGetConfigs(display,NULL,0,&numConfigs);
	assert(result != EGL_FALSE );
	std::cout<< "number of configs found "<<numConfigs<<"\n";
	EGLConfig *configs = new EGLConfig[numConfigs];
	result=eglGetConfigs(display,configs,numConfigs,&numConfigs);
	assert(result != EGL_FALSE );

	EGLint value;
	std::cout<<"--------------------------------------------------------------------------\n";

	std::cout << "BufferSz;RedSz;GreenSz;BlueSz;AlphaSz;AlphaMskSz;RGBText;RGBAText;"
				<< "COLOR_BUFFER_TYPE;"
				<< "EGL_CONFIG_CAVEAT;"
				<< "ID;zDepthSz;FBO_Level;"
				<< "MaxPbufferWidth;MaxPbufferHeight;MaxPbufferPixels;"
				<< "MaxSwapInterval;MinSwapInterval;NativeRender;"
				<< "NativeVisualID;NativeVisualType;RenderableType;"
				<< "SampleBuffers;Samples;"
				<< "StencilSz;SurfaceType;"
				<< "XType;XRedVal;XGreenVal;XBlueVal";

	for(int i=0; i<numConfigs; ++i)
	{
		eglGetConfigAttrib(display,configs[i],EGL_BUFFER_SIZE,&value);
		std::cout << value << ";";
		eglGetConfigAttrib(display,configs[i],EGL_RED_SIZE,&value);
		std::cout << value << ";";
		eglGetConfigAttrib(display,configs[i],EGL_GREEN_SIZE,&value);
		std::cout << value << ";";
		eglGetConfigAttrib(display,configs[i],EGL_BLUE_SIZE,&value);
		std::cout << value << ";";
		eglGetConfigAttrib(display,configs[i],EGL_ALPHA_SIZE,&value);
		std::cout << value << ";";
		eglGetConfigAttrib(display,configs[i],EGL_ALPHA_MASK_SIZE,&value);
		std::cout << value << ";";
		eglGetConfigAttrib(display,configs[i],EGL_BIND_TO_TEXTURE_RGB,&value);
		std::cout << std::string(value ? "true" : "false") << ";";
		eglGetConfigAttrib(display,configs[i],EGL_BIND_TO_TEXTURE_RGBA,&value);
		std::cout << std::string(value ? "true" : "false") << ";";
		eglGetConfigAttrib(display,configs[i],EGL_COLOR_BUFFER_TYPE,&value);
		std::cout << std::string(value == EGL_RGB_BUFFER ? "RGB" : "LUMINANCE") << ";";
		eglGetConfigAttrib(display,configs[i],EGL_CONFIG_CAVEAT,&value);
		std::cout << std::string(value == EGL_NONE ? "NONE" : "SLOW_CONFIG") << ";";
		eglGetConfigAttrib(display,configs[i],EGL_CONFIG_ID,&value);
		std::cout << value << ";";
		eglGetConfigAttrib(display,configs[i],EGL_DEPTH_SIZE,&value);
		std::cout << value << ";";
		eglGetConfigAttrib(display,configs[i],EGL_LEVEL,&value);
		std::cout << value << ";";
		eglGetConfigAttrib(display,configs[i],EGL_MAX_PBUFFER_WIDTH,&value);
		std::cout << value << ";";
		eglGetConfigAttrib(display,configs[i],EGL_MAX_PBUFFER_HEIGHT,&value);
		std::cout << value <<";";
		eglGetConfigAttrib(display,configs[i],EGL_MAX_PBUFFER_PIXELS,&value);
		std::cout << value << ";";
		eglGetConfigAttrib(display,configs[i],EGL_MAX_SWAP_INTERVAL,&value);
		std::cout << value << ";";
		eglGetConfigAttrib(display,configs[i],EGL_MIN_SWAP_INTERVAL,&value);
		std::cout << value << ";";
		eglGetConfigAttrib(display,configs[i],EGL_NATIVE_RENDERABLE,&value);
		std::cout << std::string(value ? "true" : "false") << ";";
		eglGetConfigAttrib(display,configs[i],EGL_NATIVE_VISUAL_ID,&value);
		std::cout << value << ";";
		eglGetConfigAttrib(display,configs[i],EGL_NATIVE_VISUAL_TYPE,&value);
		std::cout << value << ";";
		eglGetConfigAttrib(display,configs[i],EGL_RENDERABLE_TYPE,&value);
		std::cout << std::hex << value << std::dec << ";";
		eglGetConfigAttrib(display,configs[i],EGL_SAMPLE_BUFFERS,&value);
		std::cout << value << ";";
		eglGetConfigAttrib(display,configs[i],EGL_SAMPLES,&value);
		std::cout << value << ";";
		eglGetConfigAttrib(display,configs[i],EGL_STENCIL_SIZE,&value);
		std::cout << value << ";";
		eglGetConfigAttrib(display,configs[i],EGL_SURFACE_TYPE,&value);
		std::cout << std::hex << value << std::dec << ";";
		eglGetConfigAttrib(display,configs[i],EGL_TRANSPARENT_TYPE,&value);
		std::cout << std::string(value == EGL_NONE ? "EGL_NONE" : "EGL_TRANSPARENT_RGB") << ";";
		eglGetConfigAttrib(display,configs[i],EGL_TRANSPARENT_RED_VALUE,&value);
		std::cout << value << ";";
		eglGetConfigAttrib(display,configs[i],EGL_TRANSPARENT_GREEN_VALUE,&value);
		std::cout << value << ";";
		eglGetConfigAttrib(display,configs[i],EGL_TRANSPARENT_BLUE_VALUE,&value);
		std::cout << value << ";";

		std::cout << std::endl;

	}
	// now clear up the configs now we have done
	delete [] configs;

}
