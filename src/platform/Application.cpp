#include "Application.h"
#include "Renderer.h"
#include "Logger.h"
#include "Consts.h"

#define _USE_MATH_DEFINES
#include "math.h"
#include "string.h"

#include "GL/glew.h"


// PUBLIC INTERFACE

bool Application::Init(Renderer *inrenderer, const char * commandLine)
{
	renderer = inrenderer;

    ViewPos.x = ViewPos.y = 0;

    DTAccumulated = 0;
 
    return true;
}


void Application::Render()
{
    float r = (float)rand() / RAND_MAX;
    float g = (float)rand() / RAND_MAX;
    float b = (float)rand() / RAND_MAX;

    glClearColor(r, g, b, 1);
	renderer->BeginRender();

    glColor3f(1, 1, 1);

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0, 0);
    glVertex2f(0, 1);
    glVertex2f(1, 1);
    glVertex2f(1, 0);
    glEnd();
    

	renderer->EndRender();
}


void Application::Update(float dt)
{
    
}



Application::~Application()
{
    
}

// PRIVATE INTERFACE