#include "Application.h"
#include "Renderer.h"
#include "Logger.h"
#include "Consts.h"
#include "Input.h"

#define _USE_MATH_DEFINES
#include "math.h"
#include "string.h"

#include "GL/glew.h"

#include "fgl/Global.h"

#include "fgl/Triangle.h"
#include "fgl/GridCalc.h"

//Triangle tri;

GridCalc grid(Consts::DefaultSceneWidth, Consts::DefaultSceneHeight);
float *buffers[3];

// PUBLIC INTERFACE

bool Application::Init(Renderer *inrenderer, const char * commandLine)
{
	renderer = inrenderer;

    ViewPos.x = ViewPos.y = 0;

    DTAccumulated = 0;

    grid.SetTexture(Global::Instance().GetTriTexture());
    int theId = -1;
    buffers[0] = grid.GetBuffer(theId);
    theId = -1;
    buffers[1] = grid.GetBuffer(theId);
    theId = -1;
    buffers[2] = grid.GetBuffer(theId);

    for (unsigned int i = 0; i < Consts::DefaultSceneWidth * Consts::DefaultSceneHeight; ++i)
    {
        buffers[0][i] = 0.0f;
        buffers[1][i] = 0.0f;
        buffers[2][i] = 0.0f;
    }

    buffers[1][Consts::DefaultSceneWidth / 2 * Consts::DefaultSceneWidth + Consts::DefaultSceneWidth / 2] = 10.0f;
    buffers[0][Consts::DefaultSceneWidth / 2 * Consts::DefaultSceneWidth + Consts::DefaultSceneWidth / 2] = 10.00f;
    //buffers[1][256 * 512 + 255] = 0.1f;
    //buffers[1][255 * 512 + 256] = 0.1f;
    //buffers[1][255 * 512 + 255] = 0.1f;

    grid.UpdateBuffer(0);
    grid.UpdateBuffer(1);
    grid.UpdateBuffer(2);

 
    return true;
}


void Application::Render()
{
    //float r = (float)rand() / RAND_MAX;
    //float g = (float)rand() / RAND_MAX;
    //float b = (float)rand() / RAND_MAX;

    if (Input::MouseUp[0])
    {
        int lastbuf = (int)grid.GetLastBuffer();
        int buf1Ind = (lastbuf + 1) % 3;
        //int buf1Ind = (lastbuf + 1) % 3;
        float *buf = grid.GetBuffer(buf1Ind);
        int ind = (Consts::DefaultSceneHeight - Input::MouseY) * Consts::DefaultSceneWidth + Input::MouseX;
        float add = -4.2f;
        buf[ind] += add;
        /*buf[ind + 1] += add;
        buf[ind + 2] += add;
        buf[ind + 3] += add;
        buf[ind + 4] += add;
        buf[ind + 5] += add;*/
        /*float maxv = -1000.0f;
        float minv = 1000.0f;
        for (unsigned int i = 0; i < W * H; ++i)
        {
            if (buf[i] < minv)
            {
                minv = buf[i];
            }

            if (buf[i] > maxv)
            {
                maxv = buf[i];
            }
            //buf[i] = 0.0;
        }*/
        grid.UpdateBuffer(buf1Ind);

        //Sleep(50);
    }

    //glClearColor(r, g, b, 1);
	renderer->BeginRender();
    //Sleep(50);

    
    
	//tri.Render();
    grid.Calculate();

	renderer->EndRender();
}


void Application::Update(float dt)
{
    
}



Application::~Application()
{
    
}

// PRIVATE INTERFACE