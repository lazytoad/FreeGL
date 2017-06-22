#include "Global.h"

#include "platform/Utils.h"

#include <stdlib.h>

static Global _instance;

Global &Global::Instance()
{
	return _instance;
}

void Global::Initialize()
{
	framebuffer = genTexture();
	glBindImageTexture(0, framebuffer, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);

	primitives[TRIANGLE].createFromFile("res/triangle.prog");

	triTexture.Create(512, 512);
	triTexture.FillChecker();

	/*Program prg;
	prg.createFromFile("res/test.txt");

	unsigned size = 5 * 1024 * 1024;// 25 * 1024 * 1024;
	unsigned *data = new unsigned[size];

	unsigned chk = 0;

	for (unsigned i = 0; i < size; ++i)
	{
		data[i] = rand() * rand();
		chk = chk ^ data[i];
	}

	GLuint buf;
	GLuint out;

	glGenBuffers(1, &buf);
	glGenBuffers(1, &out);

	glUseProgram(prg.program);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, out);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, out);

	glBufferData(GL_SHADER_STORAGE_BUFFER, 40, 0, GL_DYNAMIC_DRAW);
	GLuint zero[3] = { 0, 0, 0};
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, 40, zero);
	checkGLErrors("Dispatch compute shader0");

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, buf);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, buf);

	glBufferData(GL_SHADER_STORAGE_BUFFER, size * 4, data, GL_STATIC_COPY);
	
	//glMemoryBarrier(GL_ALL_BARRIER_BITS);
	glDispatchCompute(1, 1, 1); // 512^2 threads in blocks of 16^2
	checkGLErrors("Dispatch compute shader1");

	glMemoryBarrier(GL_ALL_BARRIER_BITS);//GL_SHADER_STORAGE_BARRIER_BIT);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, out);

	GLint *ptr = (GLint *)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
	GLint  check[3] = {ptr[0], ptr[1], ptr[2]};
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	checkGLErrors("Dispatch compute shader2");

	if (check[2] != chk)
	{
		printf("tt");
	}*/

}

void Global::SetViewport(float _minX, float _minY, float _maxX, float _maxY)
{
	minX = _minX;
	minY = _minY;
	maxX = _maxX;
	maxY = _maxY;
}

void Global::SetFrameBuffer(unsigned width, unsigned height)
{
	frameWidth = width;
	frameHeight = height;
}