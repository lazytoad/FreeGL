#include "TextureBuffer.h"
#include "platform/Utils.h"


bool TextureBuffer::Create(unsigned _width, unsigned _height)
{
	texture = genTexture(width, height);

	width = _width;
	height = _height;
	checkGLErrors("texture create");
	return texture != 0;
}

void TextureBuffer::Fill(void * data)
{
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RED, GL_FLOAT, data);
}

void TextureBuffer::FillChecker()
{
	unsigned char *data = new unsigned char[width * height * 4];

	int stripes = 40;

	for (unsigned j = 0; j < height; ++j)
	{
		for (unsigned i = 0; i < width; ++i)
		{
			unsigned char value = ((j / stripes) % 2) * 255;
			data[ (i + j * width) * 4 + 0 ] = value;
			data[ (i + j * width) * 4 + 1 ] = value;
			data[ (i + j * width) * 4 + 2 ] = value;
			data[ (i + j * width) * 4 + 3 ] = 1;
		}
	}

	Fill(data);

	delete[] data;
}