#pragma once

#include "GL/glew.h"

struct Program
{
public:
    Program();
    ~Program();
    GLuint create(const char * source);
    GLuint createFromFile(const char * fileName);

	GLuint program;

};