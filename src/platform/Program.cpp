#include "Program.h"
#include "Logger.h"

#include <memory.h>

#define MAX_COMPUTE_WORK_GROUP_INVOCATIONS 0x90EB

GLuint Program::create(const char * source)
{
    // Creating the compute shader, and the program object containing the shader
    GLuint progHandle = glCreateProgram();
    GLuint cs = glCreateShader(GL_COMPUTE_SHADER);
    int num;
    glGetIntegerv(MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &num);

    // In order to write to a texture, we have to introduce it as image2D.
    // local_size_x/y/z layout variables define the work group size.
    // gl_GlobalInvocationID is a uvec3 variable giving the global ID of the thread,
    // gl_LocalInvocationID is the local index within the work group, and
    // gl_WorkGroupID is the work group's index
    /*const char *csSrc[] = {
        "#version 430\n",
        "uniform float roll;\
         uniform image2D destTex;\
         layout (local_size_x = 16, local_size_y = 16) in;\
         void main() {\
             ivec2 storePos = ivec2(gl_GlobalInvocationID.xy);\
             float localCoef = length(vec2(ivec2(gl_LocalInvocationID.xy)-8)/8.0);\
             float globalCoef = sin(float(gl_WorkGroupID.x+gl_WorkGroupID.y)*0.1 + roll)*0.5;\
             imageStore(destTex, storePos, vec4(1.0-globalCoef*localCoef, 0.0, 0.0, 0.0));\
         }"
    };*/
    const char *data[] = { "#version 430\n", source };
    glShaderSource(cs, 2, data, nullptr);
    glCompileShader(cs);
    int rvalue;
    glGetShaderiv(cs, GL_COMPILE_STATUS, &rvalue);
    if (!rvalue) {
        LOG_ERROR "Error in compiling the compute shader\n" LOG_END
            GLchar log[10240];
        GLsizei length;
        glGetShaderInfoLog(cs, 10239, &length, log);
        LOG_ERROR "Compiler log:" << log LOG_END;
        program = 0;
        return program;
    }
    glAttachShader(progHandle, cs);

    glLinkProgram(progHandle);
    glGetProgramiv(progHandle, GL_LINK_STATUS, &rvalue);
    if (!rvalue) {
        LOG_ERROR "Error in linking compute shader program\n" LOG_END;
        GLchar log[10240];
        GLsizei length;
        glGetProgramInfoLog(progHandle, 10239, &length, log);
        LOG_ERROR "Linker log:\n" << log LOG_END;
        program = 0;
        return program;
    }
    glUseProgram(progHandle);
	//glUniform1i(glGetUniformLocation(progHandle, "destTex"), 0);

    program = progHandle;

    //checkErrors("Compute shader");
    return progHandle;
}

GLuint Program::createFromFile(const char * fileName, const std::map<std::string, std::string> *replace)
{
	FILE *file = fopen(fileName, "r");
	if (nullptr == file)
	{
		LOG_ERROR "file not found! " << fileName LOG_END;
		return 0;
	}

	fseek(file, 0, SEEK_END);
	auto size = ftell(file);
	fseek(file, 0, SEEK_SET);
	char * contents = (char*)malloc(size + 1);
	auto numsize = fread(contents, 1, size, file);
	if (numsize < 1)
	{
		fclose(file);
		free(contents);
		LOG_ERROR "file read failed! " << fileName LOG_END;
		return 0;
	}
	contents[numsize] = '\0';
	std::string str = contents;
	free(contents);
	fclose(file);

	if (replace != nullptr)
	{
		replaceMacro(str, *replace);
	}
	GLuint result = create(str.c_str());
	
    return result;
}

void Program::replaceMacro(std::string &str, const std::map<std::string, std::string> &macro)
{
	for (const auto &pair : macro)
	{
		std::string::size_type pos = 0;
		while ((pos = str.find(pair.first, pos)) != std::string::npos) {
			str.replace(pos, pair.first.length(), pair.second);
			pos += pair.second.length(); // Handles case where 'to' is a substring of 'from'
		}
	}
}

Program::Program()
{
    program = 0;
}

Program::~Program()
{
    glDeleteProgram(program);
}