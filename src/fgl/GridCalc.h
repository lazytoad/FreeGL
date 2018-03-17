#pragma once

#include <vector>
#include <unordered_map>
#include <memory>

class TextureBuffer;

class GridCalc
{
public:
    GridCalc(int _width, int _height);
    ~GridCalc() = default;

    void SetTexture(TextureBuffer * _texture)
    {
        texture = _texture;
    }
    float *GetBuffer(int &id);
    void UpdateBuffer(int id);

    void SetProgram();

    void Calculate();

    unsigned int GetLastBuffer()
    {
        return lastBuffer;
    }

protected:

    struct GPUBuffer
    {
        std::vector<float> buffer;
        unsigned int id;

        ~GPUBuffer();
    };

    int width;
    int height;

    unsigned int lastBuffer;

    std::vector<std::unique_ptr<GPUBuffer> > buffers;
    TextureBuffer * texture;


};