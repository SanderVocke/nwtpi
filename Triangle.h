#ifndef _TRIANGLE_H
#define _TRIANGLE_H

#include <string>

//#include "GLES/gl.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"
#include "GLES2/gl2.h"

#include "nwtpi.h"

using namespace std;

class Triangle {
	private:
   		//EGLContext 	context;
		//GLDrawable drawable;
		NWTPI *drawable;
		EglState *egl;
	
		int width;
		int height;
    
    	static const GLfloat triangleVertices[];
	    static const GLubyte triangleIndices[];
		static const int numVertices;
		static const int numIndices;

    	static const char vshTriangleSource[];
		static const char fshTriangleSource[];
	
    	GLuint 		frameBufferId;
    	GLuint 		renderBufferId;
    	GLuint* 	vboIds;
    	GLuint 		programId;
    
 		// Attributes and Uniforms locations.
	 	int			uniforms[2];
 		int			attributes[2];
 	
 		enum { A_VERTEX=0, A_TEXCOORD };
		enum { U_MVPMATRIX=0, U_MAP };
	
		void 		createBufferObjects();
		GLuint* 	newBufferObject(const GLfloat* verticesArray, const GLubyte* indicesArray);
//		void 	createFrameAndRenderbuffers();
    	GLuint 	newShader(GLenum type, const char * source);
    	GLuint 	newProgram(GLuint vertexShader, GLuint fragmentShader);
		bool 	createProgramAndShaders();
		void 	checkShaderLogInfo(GLuint shaderId);
		void 	checkProgramLogInfo(GLuint programId);
		void 	checkGLError(string);
		bool 	isVBOBinded(GLenum type, int vboId);
		void 	checkVBOBinding(GLenum type, int vboId);
//		void 	tWait();

	public:
		Triangle();
		Triangle(EglState *);
		Triangle(NWTPI *);
		~Triangle();
		void 	drawScene();
		void 	clearBackground(float blueColor);
		
};

#endif
