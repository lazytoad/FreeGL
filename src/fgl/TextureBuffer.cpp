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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	checkGLErrors("texture fill");
}

void TextureBuffer::FillChecker()
{
	unsigned char *data = new unsigned char[width * height * 4];

	int stripes = 40;

	for (unsigned j = 0; j < height; ++j)
	{
		for (unsigned i = 0; i < width; ++i)
		{
			unsigned char value = 255 - ((((j / stripes) % 2) + ((i / stripes) % 2)) %2)* 128;
			data[ (i + j * width) * 4 + 0 ] = value;
			data[ (i + j * width) * 4 + 1 ] = value;
			data[ (i + j * width) * 4 + 2 ] = value;
			data[ (i + j * width) * 4 + 3 ] = 1;
		}
	}

	Fill(data);

	delete[] data;
}