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

	std::vector<Triangle::TexturedVertex> vertices;

	int bigSectors = 30;
	int smallSectors = 60;
	float bigRadius = 0.7f;
	float smallRadius = 0.3f;

	for (int i = 0; i < bigSectors; ++i)
	{
		float phaseBig0 = M_PI * 2 * i / bigSectors;
		float phaseBig1 = M_PI * 2 * (i + 1) / bigSectors;

		
		for (int j = 0; j < smallSectors; ++j)
		{
			float phaseSmall0 = M_PI * 2 * j / smallSectors;
			float phaseSmall1 = M_PI * 2 * (j + 1) / smallSectors;

			Triangle::TexturedVertex vertex;
			vertex.t[0] = 1.0f * j / smallSectors;
			vertex.t[1] = 1.0f * i / bigSectors;

			vertex.v[0] = cosf(phaseBig0) * (bigRadius + smallRadius * cosf(phaseSmall0));
			vertex.v[1] = sinf(phaseBig0) * (bigRadius + smallRadius * cosf(phaseSmall0));
			vertex.v[2] = smallRadius * sinf(phaseSmall0);

			vertices.emplace_back(vertex); // 0, 0

			vertex.t[0] = 1.0f * (j + 1) / smallSectors;
			vertex.t[1] = 1.0f * i / bigSectors;

			vertex.v[0] = cosf(phaseBig0) * (bigRadius + smallRadius * cosf(phaseSmall1));
			vertex.v[1] = sinf(phaseBig0) * (bigRadius + smallRadius * cosf(phaseSmall1));
			vertex.v[2] = smallRadius * sinf(phaseSmall1);

			vertices.emplace_back(vertex); // 0, 1

			vertex.t[0] = 1.0f * (j + 1) / smallSectors;
			vertex.t[1] = 1.0f * (i + 1) / bigSectors;

			vertex.v[0] = cosf(phaseBig1) * (bigRadius + smallRadius * cosf(phaseSmall1));
			vertex.v[1] = sinf(phaseBig1) * (bigRadius + smallRadius * cosf(phaseSmall1));
			vertex.v[2] = smallRadius * sinf(phaseSmall1);

			vertices.emplace_back(vertex); // 1, 1


			vertex.t[0] = 1.0f * j / smallSectors;
			vertex.t[1] = 1.0f * i / bigSectors;

			vertex.v[0] = cosf(phaseBig0) * (bigRadius + smallRadius * cosf(phaseSmall0));
			vertex.v[1] = sinf(phaseBig0) * (bigRadius + smallRadius * cosf(phaseSmall0));
			vertex.v[2] = smallRadius * sinf(phaseSmall0);

			vertices.emplace_back(vertex); // 0, 0

			vertex.t[0] = 1.0f * (j + 1) / smallSectors;
			vertex.t[1] = 1.0f * (i + 1) / bigSectors;

			vertex.v[0] = cosf(phaseBig1) * (bigRadius + smallRadius * cosf(phaseSmall1));
			vertex.v[1] = sinf(phaseBig1) * (bigRadius + smallRadius * cosf(phaseSmall1));
			vertex.v[2] = smallRadius * sinf(phaseSmall1);

			vertices.emplace_back(vertex); // 1, 1

			vertex.t[0] = 1.0f * j / smallSectors;
			vertex.t[1] = 1.0f * (i + 1) / bigSectors;

			vertex.v[0] = cosf(phaseBig1) * (bigRadius + smallRadius * cosf(phaseSmall0));
			vertex.v[1] = sinf(phaseBig1) * (bigRadius + smallRadius * cosf(phaseSmall0));
			vertex.v[2] = smallRadius * sinf(phaseSmall0);

			vertices.emplace_back(vertex); // 1, 0

			
		}
	}

	Triangle::TexturedVertex vvertices[] = 
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

	//tri.SetData(vvertices, sizeof(vvertices) / sizeof(Triangle::TexturedVertex) / 3);
	tri.SetData(vertices.data(), vertices.size() / 3);
 
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