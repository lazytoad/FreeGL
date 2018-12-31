#include "Triangle.h"
#include "TextureBuffer.h"

#include "platform/Utils.h"
#include "Global.h"

#include "platform/Utils.h"

// \todo delete. move to app
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace
{
    struct renderInfo
    {
        int bot, mid, top, isLeft;
        float arg1, arg2;
        float p20[2], p10[2];
    };
}

Triangle::Triangle()
{
    dataBuffer = 0;
    rasterBuffer = 0;
}

Triangle::~Triangle()
{
    glDeleteBuffers(1, &rasterBuffer);
    glDeleteBuffers(1, &dataBuffer);
}

void Triangle::Initialize()
{
    //mat.Translation({ 300.0f, 300.0f, 0.0f }, mat);

	glGenBuffers(1, &dataBuffer);
    glGenBuffers(1, &rasterBuffer);

	//glBindBuffer(GL_SHADER_STORAGE_BUFFER, dataBuffer);
	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, dataBuffer);
}

void Triangle::SetData(void * buffer, size_t _size)
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, dataBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, 15 * 4 * _size, buffer, GL_STATIC_COPY);
	size = _size;
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, rasterBuffer);
    glBufferStorage(GL_SHADER_STORAGE_BUFFER, _size * sizeof(renderInfo),
        nullptr, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT); // resize the buffer
}

void Triangle::Render()
{
	glUseProgram(Global::Instance().GetProgram(Global::TRIANGLE));

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, dataBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, dataBuffer);
	glBindTexture(GL_TEXTURE_2D, texture->texture);
	glBindImageTexture(2, texture->texture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, Global::Instance().GetZBuffer());
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, Global::Instance().GetZBuffer());

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, rasterBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, rasterBuffer);

	glBindTexture(GL_TEXTURE_2D, Global::Instance().GetFrameBuffer());
	glBindImageTexture(0, Global::Instance().GetFrameBuffer(), 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);

    glUniform1ui(glGetUniformLocation(Global::Instance().GetProgram(Global::TRIANGLE), "tSz"), size);

    glUniformMatrix4fv(glGetUniformLocation(Global::Instance().GetProgram(Global::TRIANGLE), "transform"), 1, true, matrix);
    int groups = Global::Instance().getNumLocalGroups();
	glDispatchCompute((size + groups - 1) / groups, 1, 1);
    //glDispatchCompute(groups, 1, 1);
    //glDispatchCompute(size, 1, 1);
    //glDispatchCompute(1, 1, 1);

    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT );
    glUseProgram(Global::Instance().GetProgram(Global::TRI_RASTER));


    /*glBindBuffer(GL_SHADER_STORAGE_BUFFER, dataBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, dataBuffer);
    glBindTexture(GL_TEXTURE_2D, texture->texture);
    glBindImageTexture(2, texture->texture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, Global::Instance().GetZBuffer());
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, Global::Instance().GetZBuffer());

    glBindTexture(GL_TEXTURE_2D, Global::Instance().GetFrameBuffer());
    glBindImageTexture(0, Global::Instance().GetFrameBuffer(), 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);*/

    glUniformMatrix4fv(glGetUniformLocation(Global::Instance().GetProgram(Global::TRI_RASTER), "transform"), 1, true, matrix);

    glDispatchCompute(1, 1, 1);
	

	//glMemoryBarrier(GL_ALL_BARRIER_BITS);

	/*glBindBuffer(GL_SHADER_STORAGE_BUFFER, tmp);

	float *ptr = (float *)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
	float  check[40];
	memcpy(check, ptr, 4 * 40);
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);*/

	checkGLErrors("tri end");
}
