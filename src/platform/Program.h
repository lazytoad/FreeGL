#pragma once

#include "GL/glew.h"

class Program
{
public:
    Program();
    ~Program();
    GLuint create(const char * source);
    GLuint createFromFile(const char * fileName);

	GLuint program;

};