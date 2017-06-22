#include "Triangle.h"
#include "TextureBuffer.h"

#include "platform/Utils.h"

Triangle::Triangle()
{
	glGenBuffers(1, &dataBuffer);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, dataBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, dataBuffer);
}

void Triangle::SetData(TexturedVertex * buffer, size_t size)
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, dataBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size * sizeof(TexturedVertex), buffer, GL_STATIC_COPY);
}

void Triangle::Render()
{
	glBindTexture(GL_TEXTURE_2D, texture->texture);
	glBindImageTexture(1, texture->texture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
}
