#pragma once

#include <string>

#include "GL/glew.h"

bool checkGLErrors(const char *string);
bool checkGLErrors(const std::string &string);

GLuint genTexture(unsigned width = 512, unsigned height = 512);