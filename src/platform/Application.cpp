#include "Application.h"
#include "Renderer.h"
#include "Logger.h"
#include "Consts.h"

#define _USE_MATH_DEFINES
#include "math.h"
#include "string.h"

#include "GL/glew.h"

#include "fgl/Global.h"

#include "fgl/Triangle.h"

Triangle tri;

// PUBLIC INTERFACE

bool Application::Init(Renderer *inrenderer, const char * commandLine)
{
	renderer = inrenderer;

    ViewPos.x = ViewPos.y = 0;

    DTAccumulated = 0;
	tri.Initialize();
	tri.SetTexture(Global::Instance().GetTriTexture());

	Triangle::TexturedVertex vertices[] = 
	{
		{
			{0.1f, 0.1f, 0},
			{0, 0},
			//1
		},
		{
			{ 0.9f, 0.1f, 0 },
			{ 1, 0 },
			//1
		},
		{
			{ 0.9f, 0.9f, 0 },
			{ 1, 1 },
			//1
		},

		{
			{0.1f, 0.1f, 0},
			{0, 0},
			//1
		},
		{
			{ 0.9f, 0.9f, 0 },
			{ 1, 1 },
			//1
		},
		{
			{ 0.1f, 0.9f, 0 },
			{ 0, 1 },
			//1
		}
	};

	//Triangle::TriStruct T;
	//memcpy(&T, vertices, sizeof(T));

	tri.SetData(vertices, sizeof(vertices) / sizeof(Triangle::TexturedVertex) / 3);
 
    return true;
}


void Application::Render()
{
    float r = (float)rand() / RAND_MAX;
    float g = (float)rand() / RAND_MAX;
    float b = (float)rand() / RAND_MAX;

    //glClearColor(r, g, b, 1);
	renderer->BeginRender();
    
	tri.Render();

	renderer->EndRender();
}


void Application::Update(float dt)
{
    
}



Application::~Application()
{
    
}

// PRIVATE INTERFACE