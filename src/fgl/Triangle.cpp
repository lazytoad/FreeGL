#include "Triangle.h"
#include "TextureBuffer.h"

#include "platform/Utils.h"
#include "Global.h"

#include "platform/Utils.h"

Triangle::Triangle()
{
	
}

void Triangle::Initialize()
{
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

	glBindTexture(GL_TEXTURE_2D, texture->texture);
	glBindImageTexture(2, texture->texture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);

	/*unsigned char *u = new unsigned char[512*512*4];
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, u);

	unsigned char val = u[0];
	delete[] u;*/

	glBindTexture(GL_TEXTURE_2D, Global::Instance().GetFrameBuffer());
	glBindImageTexture(0, Global::Instance().GetFrameBuffer(), 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);

	//auto uloc = glGetUniformLocation(Global::Instance().GetProgram(Global::TRIANGLE), "add");
	auto loc = glGetUniformLocation(Global::Instance().GetProgram(Global::TRIANGLE), "add");
	glUniform2f(loc, 256, 256);
	glUniform2f(glGetUniformLocation(Global::Instance().GetProgram(Global::TRIANGLE), "scale"), 256, 256);
	glDispatchCompute((size + 1024 - 1) / 1024, 1, 1);
	

	//glMemoryBarrier(GL_ALL_BARRIER_BITS);

	/*glBindBuffer(GL_SHADER_STORAGE_BUFFER, tmp);

	float *ptr = (float *)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
	float  check[40];
	memcpy(check, ptr, 4 * 40);
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);*/

	checkGLErrors("tri end");
}
