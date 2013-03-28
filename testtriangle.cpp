/* {{{1
Copyright (c) 2012, vslash.com
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

#include <stdio.h>
#include <stdarg.h>
#include <sys/time.h>

#include <iostream>

#include "bcm_host.h"

#include "GLES2/gl2.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"

#include "nwtpi.h"
#include "misc.h"
#include "Triangle.h"

using namespace std;

static EglState _egls, *egls=&_egls;

static volatile int terminate;

//static void 		drawScene(EglState *);
//static void			clearBackground(float);

//static void drawScene(EglState *egl)/*{{{*/
//{
//   // Start with a clear screen
//   glClear( GL_COLOR_BUFFER_BIT );
//   glMatrixMode(GL_MODELVIEW);
//
//   glEnable(GL_TEXTURE_2D);
//   glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
//
//   // Draw first (front) face:
//   // Bind texture surface to current vertices
//   glBindTexture(GL_TEXTURE_2D, dispman->tex[0]);
//
//   // Need to rotate textures - do this by rotating each cube face
//   glRotatef(270.f, 0.f, 0.f, 1.f ); // front face normal along z axis
//
//   // draw first 4 vertices
//   glDrawArrays( GL_TRIANGLE_STRIP, 0, 4);
//
//   // same pattern for other 5 faces - rotation chosen to make image orientation 'nice'
//   glBindTexture(GL_TEXTURE_2D, dispman->tex[1]);
//   glRotatef(90.f, 0.f, 0.f, 1.f ); // back face normal along z axis
//   glDrawArrays( GL_TRIANGLE_STRIP, 4, 4);
//
//   glBindTexture(GL_TEXTURE_2D, dispman->tex[2]);
//   glRotatef(90.f, 1.f, 0.f, 0.f ); // left face normal along x axis
//   glDrawArrays( GL_TRIANGLE_STRIP, 8, 4);
//
//   glBindTexture(GL_TEXTURE_2D, dispman->tex[3]);
//   glRotatef(90.f, 1.f, 0.f, 0.f ); // right face normal along x axis
//   glDrawArrays( GL_TRIANGLE_STRIP, 12, 4);
//
//   glBindTexture(GL_TEXTURE_2D, dispman->tex[4]);
//   glRotatef(270.f, 0.f, 1.f, 0.f ); // top face normal along y axis
//   glDrawArrays( GL_TRIANGLE_STRIP, 16, 4);
//
//   glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//
//   glBindTexture(GL_TEXTURE_2D, dispman->tex[5]);
//   glRotatef(90.f, 0.f, 1.f, 0.f ); // bottom face normal along y axis
//   glDrawArrays( GL_TRIANGLE_STRIP, 20, 4);
//
//   glDisable(GL_TEXTURE_2D);
//
//   eglSwapBuffers(egl->display, egl->surface);
//}/*}}}*/

//static void clearBackground(float blueColor) {/*{{{*/
//	glClearColor(0.2f, 0.3f, blueColor, 0.9f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//}/*}}}*/


int main(int argc,char* argv[])
{
    struct timeval t1, t2;
    struct timezone tz;
    float deltatime;
    float totaltime = 0.0f;
    unsigned int frames = 0;
	
	try
	{
//		cout << "==> bcm init : ";
//		bcm_host_init();
//		cout << "done." << endl;
//		memset( egls, 0, sizeof( *egls ) );
//
//		// Get Display, Context, Surface, W, H and make context current
//		cout << "==> egl init : " << endl;
//		eglInit(egls);
//		cout << "==> done." << endl;

		cout << "==> Create NWTPI drawable" << endl;
		NWTPI *  drawable = new NWTPI("Triangle",1920,1080);

		cout << "==> Triangle init : " << endl;
//		Triangle* triangle = new Triangle(egls);
		Triangle* triangle = new Triangle(drawable);
		cout << "==> done." << endl;

		gettimeofday ( &t1 , &tz );
		while ( true ) 
		{
			gettimeofday(&t2, &tz);
			deltatime = (float)(t2.tv_sec - t1.tv_sec + (t2.tv_usec - t1.tv_usec) * 1e-6);
			t1 = t2;

			triangle->drawScene();

			//eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);

			totaltime += deltatime;
			frames++;
			if (totaltime >  2.0f)
			{
				printf("%4d frames rendered in %1.4f seconds -> FPS=%3.4f\n", frames, totaltime, frames/totaltime);
				totaltime -= 2.0f;
				frames = 0;
			}
    	}

		// Setup the model world
		//   init_model_proj(elgs);
		// initialise the GLES texture(s)
		//   init_textures(egls);

		//usleep(5*1000);
		//updateModel(egls);
		//drawScene(egls);
		
		eglExit(egls);
		return 0;
	}
	catch(exception& e) 								//std::runtime_error err)
	{
		/* Print error message: */
		cerr << "\n\n Exception trap : " << e.what() << "\n\n" << endl;

		/* Signal error to OS: */
		return 1;
	}
}

// vi: set foldmethod=marker:
