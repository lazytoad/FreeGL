#pragma once

#include <string>

#include "GL/glew.h"

bool checkGLErrors(const char *string);
bool checkGLErrors(const std::string &string);

GLuint genTexture();