#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>

#include <stdio.h>

#include "Triangle.h"

const GLfloat Triangle::triangleVertices[] = 
            {
                     0.0f, 1.0f, 0.0f, 			// 0
                    -0.5f, 0.0f, 0.0f, 			// 1
                     0.5f, 0.0f, 0.0f			// 2
//                     1.0f, 1.0f, 0.0f			// 3
            };
//  								       0 1 2
const GLubyte Triangle::triangleIndices[] = {0,1,2};

const int Triangle::numVertices = 3;
const int Triangle::numIndices  = 3;

const char Triangle::vshTriangleSource[] = 
	"attribute vec3 a_vertex; \n"
	"void main() \n"
	"{ \n"
	" gl_Position = vec4(a_vertex, 1.0); \n"
	"} \n";

const char Triangle::fshTriangleSource[] = 
	"void main() \n"
	"{ gl_FragColor = vec4 (1.0, 1.0, 1.0, 0.5); \n"
	"}\n";


Triangle::Triangle() {
	createBufferObjects();
	if ( createProgramAndShaders() == false)
		cout << "==> Triangle::createProgramAndShaders : NOK" << endl;
}

//Triangle::Triangle(NWTPI* _drawable) {
//	drawable = _drawable;
//	createBufferObjects();
//	if ( createProgramAndShaders() == false)
//		cout << "==> Triangle::createProgramAndShaders : NOK" << endl;
//}
Triangle::~Triangle() {
}

void Triangle::createBufferObjects() {/*{{{*/
		
	vboIds = newBufferObject(triangleVertices, triangleIndices);

}
	
GLuint* Triangle::newBufferObject(const GLfloat* verticesArray, const GLubyte* indicesArray)
{

	static GLuint tempIds[] = {0, 0};								// store temporary id's, glgenBuffers require an array

	glGenBuffers(2, tempIds);										// Generates the vertex buffer object (VBO)

	glBindBuffer(GL_ARRAY_BUFFER, tempIds[0]); // Bind the VBO so we can fill it with data
	checkVBOBinding(GL_ARRAY_BUFFER_BINDING, tempIds[0]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(GLfloat) * 3,
			verticesArray, GL_STATIC_DRAW);

	checkGLError("glBufferData");
	cout << "==> newBufferObject created (Vertices) : " << tempIds[0] << endl;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tempIds[1]);
	checkVBOBinding(GL_ELEMENT_ARRAY_BUFFER_BINDING, tempIds[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLubyte),
			indicesArray, GL_STATIC_DRAW);
	checkGLError("glBufferData");
	cout << "==> newBufferObject created (Indices ) : " << tempIds[1] << endl;

	// Unbind objects and Indices newly created
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return tempIds;
}

bool Triangle::createProgramAndShaders()
{
	GLuint vshId=-1, fshId=-1;

	// Creates the pair of Shaders.
	vshId = newShader(GL_VERTEX_SHADER, (char *) vshTriangleSource);		// or loadShaderFromFile(gl2, GL2.GL_VERTEX_SHADER, "triangle-vertex.glsl")
	fshId = newShader(GL_FRAGMENT_SHADER, (char *) fshTriangleSource);		

	// Creates the Program Object.
	if ( vshId == -1 || fshId == -1)
		return false;

	programId = newProgram(vshId, fshId);

	glDeleteShader(vshId);
	glDeleteShader(fshId);

	// Gets the attributes locations.
	attributes[A_VERTEX] = glGetAttribLocation(programId, "a_vertex");
	//attributes[1] = gl.glGetAttribLocation(programId, "a_texCoord");


	return (programId == 0 ? false : true );
}

GLuint Triangle::newProgram(GLuint vertexShader, GLuint fragmentShader)
{
	GLuint id;
	GLint linkStatus;

	// Creates the program name/index.
	if ( (id = glCreateProgram()) == 0) {
		cout << "==> newProgram failed " << endl;
		checkGLError("glCreateProgram");
		return 0;
	}

	cout << "==> newProgram created : " << id << "\n";
	// Will attach the fragment and vertex shaders to the program object.
	glAttachShader(id, vertexShader);
	glAttachShader(id, fragmentShader);

   	glBindAttribLocation(id, 0, "a_vertex");

	// Will link the program into OpenGL core.
	glLinkProgram(id);
	checkGLError("glLinkProgram");

	glGetProgramiv(id, GL_LINK_STATUS, &linkStatus);
	if ( ! linkStatus ) {
		cout << "==> newProgram : LINK_STATUS : " << linkStatus << endl;
		checkProgramLogInfo(id);
	} else
		cout << "==> newProgram linked : " << id << "\n";
	
	return id;
}

GLuint Triangle::newShader(GLenum type, const char * source)
{
	GLuint id;

	// Creates a Shader Object and returns its name/id.
	id = glCreateShader(type);

	if (id == 0 ) { 
		cout << "==> CreateShader failed" << endl;
		return id;
	}

	// Uploads the source to the Shader Object.
	//cout << "==> ShaderSource : \n" << "--\n" << source << "--\n" << endl;
	glShaderSource(id, 1, &source, NULL); // glShaderSource (GLuint shader, GLsizei count, const GLchar** string, const GLint* length);

	// Compiles the Shader Object.
	glCompileShader(id);

	int compileStatus;
	glGetShaderiv(id, GL_COMPILE_STATUS, &compileStatus); // void GetShaderiv( uint shader, enum pname, int *params );
	if ( ! compileStatus ) {
		cout << "==> newShader : COMPILE_STATUS : " << compileStatus << "\n";
		checkShaderLogInfo(id);
		return id;
	} else 
		cout << "==> newShader created : " << id << "\n";

	return id;
}

void Triangle::drawScene () 
{
	clearBackground(0.35f);

	glUseProgram(programId);

	//cout << "==> drawScene : binding objects : ";
	glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);											// Bind the Buffer Objects ==> source for vertex data
	checkVBOBinding(GL_ARRAY_BUFFER_BINDING, vboIds[0]);								// check result

	glVertexAttribPointer(attributes[A_VERTEX], 3 , GL_FLOAT, GL_FALSE, 0, (void *) 0);	// Call to VAttribPointer make really VBO as a source
	glEnableVertexAttribArray(attributes[A_VERTEX]);									// Enable our VAO Bind - VertexAttribPointer set above
   
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);
	checkVBOBinding(GL_ELEMENT_ARRAY_BUFFER_BINDING, vboIds[1]);						// check result
	//cout << "done." << endl;

	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_BYTE, (void *) 0);
	//glDrawArrays( GL_TRIANGLES, 0, 3);
	checkGLError("glDrawElements");

	glFlush();

	//drawable->swapBuffers();															// TEST OK :: XXX R35 TODO w/ EGLWindow.
	//eglSwapBuffers(drawable->egDisplay, drawable->egSurface);							// TEST OK
	//eglSwapBuffers(drawable->getCurrentDisplay(), drawable->getCurrentSurface());		// TEST OK

	glDisableVertexAttribArray(attributes[A_VERTEX]);									// Disable our Pointer
	//cout << "==> drawScene : unbinding objects : ";
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);											// unbind objects
    glBindBuffer(GL_ARRAY_BUFFER, 0);
	//cout << "done." << endl;
}

void Triangle::clearBackground (float blueColor)
{
	glClearColor(0.2f, 0.3f, blueColor, 0.9f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Triangle::checkProgramLogInfo(GLuint programId)
{
	GLsizei infoLength;

	glGetProgramiv(programId,GL_INFO_LOG_LENGTH,&infoLength);

	if ( infoLength >= 1) {
		char* infoLogBuffer = new char[infoLength];
		//glGetProgramInfoLog (GLuint program, GLsizei bufsize, GLsizei* length, GLchar* infolog);
		glGetProgramInfoLog(programId, infoLength, NULL, (GLchar *) infoLogBuffer); 
		cout << "==> Program linking result : \n>>\n" << infoLogBuffer << "\n<<\n";
		delete[] infoLogBuffer;
	}
}

void Triangle::checkShaderLogInfo(GLuint shaderId) {

	GLsizei infoLength;

	glGetShaderiv(shaderId,GL_INFO_LOG_LENGTH,&infoLength);

	if ( infoLength >= 1) {
		char* infoLogBuffer = new char[infoLength];
		glGetShaderInfoLog(shaderId, infoLength, NULL, (GLchar *) infoLogBuffer);
		cout << "==> Shader compilation result : \n>>\n" << infoLogBuffer << "\n<<\n";
		delete[] infoLogBuffer;
	}
}
	
void Triangle::checkGLError(string sFuncName )
{
	static const string errorString[] = {"INVALID_ENUM","INVALID_VALUE","INVALID_OPERATION","INVALID_FRAMEBUFFER_OPERATION","OUT_OF_MEMORY","Unknown Gl Error"};
	int glError;
	int ierr;
        do {
            glError = glGetError();
            if( glError != GL_NO_ERROR ) {
				switch (glError) {
					case GL_INVALID_ENUM: 
						ierr=0; break;
					case GL_INVALID_VALUE: 
						ierr=1; break;
					case GL_INVALID_OPERATION:
						ierr=2; break;
					case GL_INVALID_FRAMEBUFFER_OPERATION: 
						ierr=3; break;
					case GL_OUT_OF_MEMORY:
						ierr=4; break;
					default:
						ierr=5; break;
				}
                printf("%s failed, GL Error: 0x%d > %s\n",sFuncName.c_str(), glError, errorString[ierr].c_str());		// TODO : std::to_string : unknown

            }
        } while( glError != GL_NO_ERROR );
}

bool Triangle::isVBOBinded(GLenum type, int vboId) {/*{{{*/
	GLint p;
	glGetIntegerv(type,&p); 					//void glGetIntegerv(GLenum pname,  GLint * params);
	if ( p != vboId )
		return false;

	return true;
}

void Triangle::checkVBOBinding(GLenum type, int vboId) {/*{{{*/
	if ( (isVBOBinded(type, vboId)) == false )
		throw runtime_error("runtime error : Triangle::checkVBOBinding ** VBO not bound");
		//throw Exception("==> VBO not bound");
}

#ifdef DEBUG_ON
void Triangle::logd(string method, ostream& message) {
	ostringstream& s = dynamic_cast<ostringstream&>(message);
	cout << "==> " << method << " : " << s.str() << endl;
}
#endif
