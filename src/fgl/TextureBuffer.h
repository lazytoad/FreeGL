#pragma once


class TextureBuffer
{
public:
	TextureBuffer() : texture(0) {}
	bool Create(unsigned _width, unsigned _height);
	void Fill(void * data);

	void FillChecker();


	unsigned texture;
	unsigned width;
	unsigned height;

};