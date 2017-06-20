#include "Utils.h"
#include "Logger.h"

bool checkGLErrors(const char *string)
{
    GLenum e = glGetError();
    if (e != GL_NO_ERROR) {
        LOG_ERROR "OpenGL error in " << string << ":" << gluErrorString(e) << "\n code = " << e LOG_END;
        return false;
    }

    return true;
}

bool checkGLErrors(const std::string &string)
{
    return checkGLErrors(string.c_str());
}

GLuint genTexture() {
    // We create a single float channel 512^2 texture
    GLuint texHandle;
    glGenTextures(1, &texHandle);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texHandle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    

    unsigned char *buf = new unsigned char[512*512*4];
    for(unsigned char * i = buf; i != buf + 512 * 512 * 4; i += 4)
    {
        *i = 255;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 512, 512, 0, GL_RED, GL_FLOAT, buf);

    delete[] buf;

    // Because we're also using this tex as an image (in order to write to it),
    // we bind it to an image unit as well
    glBindImageTexture(0, texHandle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);

    
    checkGLErrors("texture");
    return texHandle;
}