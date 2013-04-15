/* {{{1
Copyright (c) 2013, vslash.com
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the copyright holder nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 }}} */

#include <cstdio>
#include <cstdarg>
#include <sys/time.h>

#include <iostream>

#include "bcm_host.h"

#include "GLES2/gl2.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"

#include "NWTPI.h"

#include "Triangle.h"

using namespace std;
using namespace nwtpi;

#ifdef DEBUG_ON
#define DEBUG( a, b ) logdebug(a, ostringstream().flush() << b )

void logdebug(string method, ostream& message) {
	ostringstream& s = dynamic_cast<ostringstream&>(message);
	cout << "==> " << method << " : " << s.str() << endl;
}

#else
    #define DEBUG( a, b )
#endif


int main(int argc,char* argv[])
{
    struct timeval t1, t2;
    struct timezone tz;
    float deltatime;
    float totaltime = 0.0f;
    unsigned int frames = 0;
	
	try
	{
		DEBUG ( "main()", "Create caps");
		OEGLCapabilities * caps = new OEGLCapabilities(OEGLCapabilities::RGBA888);		// getting a profile (this one will return config id #7)
		caps->setAttribute(EGL_DEPTH_SIZE,24);										// and add a conformant EGL attrib specs to our choosen profile / this one will return config id #3
		caps->setAttribute(EGL_BIND_TO_TEXTURE_RGB, 0);
		caps->setAttribute(EGL_BIND_TO_TEXTURE_RGBA, 0);							// ... and this 2 last ones will return config id #11

		DEBUG ( "main()", "Create NWTPI drawable" );
		//NWTPI *  drawable = new NWTPI("Triangle",720,480, false, caps);
		OEGLWindow * oeglWindow = new OEGLWindow("Triangle",720,480,0,caps);	// default EGLWindow is a windowSurface

		DEBUG ( "main()", "Create Triangle" );
		Triangle* triangle = new Triangle();

		gettimeofday ( &t1 , &tz );
		while ( true ) 
		{
			gettimeofday(&t2, &tz);
			deltatime = (float)(t2.tv_sec - t1.tv_sec + (t2.tv_usec - t1.tv_usec) * 1e-6);
			t1 = t2;

			triangle->drawScene();

			oeglWindow->swapBuffers();

			totaltime += deltatime;
			frames++;
			if (totaltime >  2.0f)
			{
				printf("%4d frames rendered in %1.4f seconds -> FPS=%3.4f\n", frames, totaltime, frames/totaltime);
				totaltime -= 2.0f;
				frames = 0;
			}
    	}
		
		return 0;
	}
	catch(exception& e) 													// std::runtime_error err)
	{
		cerr << "\n\n Exception trap : " << e.what() << "\n\n" << endl;
		return 1;
	}
}
