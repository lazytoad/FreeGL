#include "Program.h"
#include "Logger.h"

GLuint Program::create(const char * source)
{
    // Creating the compute shader, and the program object containing the shader
    GLuint progHandle = glCreateProgram();
    GLuint cs = glCreateShader(GL_COMPUTE_SHADER);

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

    program = progHandle;

    //glUniform1i(glGetUniformLocation(progHandle, "destTex"), 0);

    //checkErrors("Compute shader");
    return progHandle;
}

GLuint Program::createFromFile(const char * fileName)
{
    return GLuint();
}

Program::Program()
{
    program = 0;
}

Program::~Program()
{
    glDeleteProgram(program);
}