#pragma once

#include "GL/glew.h"

#include <map>
#include <string>

struct Program
{
public:
    Program();
    ~Program();
    GLuint create(const char * source);
    GLuint createFromFile(const char * fileName, const std::map<std::string, std::string> *replace = nullptr);

	GLuint program;

	private:
		void replaceMacro(std::string &str, const std::map<std::string, std::string> &macro);

};