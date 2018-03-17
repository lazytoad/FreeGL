#include "GridCalc.h"

#include "TextureBuffer.h"

#include "platform/Consts.h"
#include "platform/Utils.h"
#include "Global.h"

#include "platform/Utils.h"

GridCalc::GridCalc(int _width, int _height) : width(_width), height(_height)
{
    lastBuffer = 2;
}

float *GridCalc::GetBuffer(int &id)
{
    if (id < 0 || id >= (int)buffers.size())
    {
        id = buffers.size();
        std::unique_ptr<GPUBuffer> newBuffer = std::make_unique<GPUBuffer>();
        glGenBuffers(1, &newBuffer->id);
        newBuffer->buffer.resize(width * height);
        float *result = newBuffer->buffer.data();
        buffers.push_back(std::move(newBuffer));

        return result;
    }
    else
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffers[id]->id);
        glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, 4 * buffers[id]->buffer.size(), buffers[id]->buffer.data());
        return buffers[id]->buffer.data();
    }
}

void GridCalc::UpdateBuffer(int id)
{
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffers[id]->id);
    glBufferData(GL_SHADER_STORAGE_BUFFER, 4 * buffers[id]->buffer.size(), buffers[id]->buffer.data(), GL_STATIC_COPY);
}

void GridCalc::SetProgram()
{
}

void GridCalc::Calculate()
{
    glUseProgram(Global::Instance().GetProgram(Global::GRID));

    glBindTexture(GL_TEXTURE_2D, Global::Instance().GetFrameBuffer());
    glBindImageTexture(0, Global::Instance().GetFrameBuffer(), 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);

    unsigned int buf2 = lastBuffer;
    unsigned int buf0 = (buf2 + 1) % 3;
    unsigned int buf1 = (buf0 + 1) % 3;

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffers[buf0]->id);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, buffers[buf0]->id);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffers[buf1]->id);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, buffers[buf1]->id);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffers[buf2]->id);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, buffers[buf2]->id);

    checkGLErrors("grid calc");

	size_t total = Global::Instance().GetWidth() * Global::Instance().GetHeight();
	unsigned int numberOfRuns = total / Global::Instance().getNumLocalGroups();
	numberOfRuns += total % Global::Instance().getNumLocalGroups() == 0 ? 0 : 1;

    glDispatchCompute(numberOfRuns, 1, 1);

    checkGLErrors("grid calc 2");

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    lastBuffer = (lastBuffer + 1) % 3;
}

GridCalc::GPUBuffer::~GPUBuffer()
{
    glDeleteBuffers(1, &id);
}