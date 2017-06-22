#pragma once

class TextureBuffer;

class Triangle
{
public:
	struct TexturedVertex
	{
		float v[3];
		float t[2];
	};

	Triangle();
	void Initialize();
	void SetData(TexturedVertex *buffer, size_t elementsNumber);
	void SetTexture(TextureBuffer * _texture)
	{
		texture = _texture;
	}
	void Render();

	TextureBuffer * texture;
	unsigned dataBuffer;
};