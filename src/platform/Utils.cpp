#include "Utils.h"
#include "Logger.h"

bool checkGLErrors(const char *string)
{
    GLenum e = glGetError();
    if (e != GL_NO_ERROR) {
#ifndef NDEBUG
        LOG_ERROR "OpenGL error in " << string << ":" << gluErrorString(e) << "\n code = " << e LOG_END;
#else
		LOG_ERROR "OpenGL error in " << string << ":" << e << "\n code = " << e LOG_END;
#endif
        return false;
    }

    return true;
}

bool checkGLErrors(const std::string &string)
{
    return checkGLErrors(string.c_str());
}

GLuint genTexture(unsigned width, unsigned height) {
    // We create a single float channel 512^2 texture
    GLuint texHandle;
    glGenTextures(1, &texHandle);

    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texHandle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    

    //unsigned char *buf = new unsigned char[width *height *4];
    //for(unsigned char * i = buf; i != buf + width * height * 4; i += 4)
    //{
    //    *i = 255;
    //}

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RED, GL_FLOAT, nullptr);

    //delete[] buf;

    
    checkGLErrors("texture");
    return texHandle;
}