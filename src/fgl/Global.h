#pragma once

#include "platform/Program.h"
#include "TextureBuffer.h"

class Global
{
public:
	enum Primitive
	{
		TRIANGLE = 0,
		SPRITE	 = 1,
		PRIMITIVES_TOTAL
	};
	static Global &Instance();

	void SetViewport(float _minX, float _minY, float _maxX, float _maxY);
	void SetFrameBuffer(unsigned width, unsigned height);

	unsigned int GetProgram(Primitive primitive)
	{
		return primitives[primitive].program;
	}

	TextureBuffer *GetTriTexture()
	{
		return &triTexture;
	}

	void Initialize();
	void Shutdowm() {} // \todo make useable

private:

	unsigned frameWidth;
	unsigned frameHeight;
	float minX;
	float minY;
	float maxX;
	float maxY;

	Program primitives[PRIMITIVES_TOTAL];
	TextureBuffer triTexture;
};