#include "Triangle.h"
#include "TextureBuffer.h"

#include "platform/Utils.h"
#include "Global.h"

#include "platform/Utils.h"

// \todo delete. move to app
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

aiMatrix4x4t<float> mat;

Triangle::Triangle()
{
	
}

void Triangle::Initialize()
{
    //mat.Translation({ 300.0f, 300.0f, 0.0f }, mat);

	glGenBuffers(1, &dataBuffer);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, dataBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, dataBuffer);
}

void Triangle::SetData(void * buffer, size_t _size)
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, dataBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, 15 * 4 * _size, buffer, GL_STATIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, dataBuffer);
	size = _size;
}

void Triangle::Render()
{
	glUseProgram(Global::Instance().GetProgram(Global::TRIANGLE));

	/*unsigned tmp;
	glGenBuffers(1, &tmp);

	

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, tmp);
	glBufferData(GL_SHADER_STORAGE_BUFFER, 40 * sizeof(float), nullptr, GL_STATIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, tmp);*/

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, dataBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, dataBuffer);
	glBindTexture(GL_TEXTURE_2D, texture->texture);
	glBindImageTexture(2, texture->texture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, Global::Instance().GetZBuffer());
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, Global::Instance().GetZBuffer());

	glBindTexture(GL_TEXTURE_2D, Global::Instance().GetFrameBuffer());
	glBindImageTexture(0, Global::Instance().GetFrameBuffer(), 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);

	auto loc = glGetUniformLocation(Global::Instance().GetProgram(Global::TRIANGLE), "add");
	//glUniform2f(loc, Global::Instance().GetWidth() / 2, Global::Instance().GetHeight() / 2 - 300);
    glUniform2f(loc, 0, 0);
	glUniform2f(glGetUniformLocation(Global::Instance().GetProgram(Global::TRIANGLE), "scale"), 1, 1);

    glUniform1ui(glGetUniformLocation(Global::Instance().GetProgram(Global::TRIANGLE), "tSz"), size);

    glUniformMatrix4fv(glGetUniformLocation(Global::Instance().GetProgram(Global::TRIANGLE), "transform"), 1, true, matrix);
	//glDispatchCompute((size + 1024 - 1) / 1024, 1, 1);
    glDispatchCompute(size, 1, 1);

    
	

	//glMemoryBarrier(GL_ALL_BARRIER_BITS);

	/*glBindBuffer(GL_SHADER_STORAGE_BUFFER, tmp);

	float *ptr = (float *)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
	float  check[40];
	memcpy(check, ptr, 4 * 40);
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);*/

	checkGLErrors("tri end");
}
