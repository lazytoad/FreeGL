#pragma once


//
//#include "gl/GL.h"
//#include "GL/glext.h"
//#include <GL/gl.h>

#include "GL/glew.h"
#include "GL/glext.h"
//#include "GL/wglew.h"

#include "Program.h"

#include "fgl/TextureBuffer.h"


class RenderCore
{
public:

	void initialize();

	GLuint framebuffer;
	Program program;
};