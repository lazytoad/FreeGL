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

/* assimp include files. These three are usually needed. */
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Triangle tri;

//#define BUNNY

// PUBLIC INTERFACE

static aiMatrix4x4t<float> mat;

bool Application::Init(Renderer *inrenderer, const char * commandLine)
{
	renderer = inrenderer;

    ViewPos.x = ViewPos.y = 0;

    DTAccumulated = 0;
	tri.Initialize();
	tri.SetTexture(Global::Instance().GetTriTexture());
    std::vector<Triangle::TexturedVertex> vertices;
	

#ifdef BUNNY
    const struct aiScene* scene = aiImportFile("./res/bunny.ply", 0);
    //if (scene) {
    //    get_bounding_box(&scene_min, &scene_max);
    //    scene_center.x = (scene_min.x + scene_max.x) / 2.0f;
    //    scene_center.y = (scene_min.y + scene_max.y) / 2.0f;
    //    scene_center.z = (scene_min.z + scene_max.z) / 2.0f;
    //    return false;
    //}

    if (scene == nullptr || scene->mNumMeshes == 0)
    {
        LOG_E("Model load failed.");
        return false;
    }

    const aiMesh* mesh = scene->mMeshes[0];

    vertices.resize(mesh->mNumFaces * 3);

    for (size_t i = 0; i < mesh->mNumFaces; ++i)
    {
        if (mesh->mFaces[i].mNumIndices != 3)
        {
            LOG_E("Unsupported type of face (currently triangle only).");
            return false;
        }

        for (size_t k = 0; k < 3; ++k)
        {
            Triangle::TexturedVertex &vertex = vertices[i * 3 + k];
            vertex.t[0] = vertex.t[1] = 0.0f;
            unsigned int index = mesh->mFaces[i].mIndices[k];
            const aiVector3D aiVertex = mesh->mVertices[index];
            vertex.v[0] = aiVertex.x;
            vertex.v[1] = aiVertex.y;
            vertex.v[2] = aiVertex.z;
        }
    }
#else
	int bigSectors = 100;
	int smallSectors = 30;
	float bigRadius = 0.7f;
	float smallRadius = 0.3f;

	for (int i = 0; i < bigSectors; ++i)
	{
		float phaseBig0 = (float)M_PI * 2 * i / bigSectors;
		float phaseBig1 = (float)M_PI * 2 * (i + 1) / bigSectors;

		
		for (int j = 0; j < smallSectors; ++j)
		{
			float phaseSmall0 = (float)M_PI * 2 * j / smallSectors;
			float phaseSmall1 = (float)M_PI * 2 * (j + 1) / smallSectors;

			Triangle::TexturedVertex vertex;
			vertex.t[0] = 1.0f * j / smallSectors;
			vertex.t[1] = 1.0f * i / bigSectors;

			float mul0 = 1 + 0.5f * cosf(phaseBig0*10);
			float mul1 = 1 + 0.5f * cosf(phaseBig1*10);

			vertex.v[0] = cosf(phaseBig0) * (bigRadius + smallRadius * cosf(phaseSmall0) * mul0);
			vertex.v[1] = sinf(phaseBig0) * (bigRadius + smallRadius * cosf(phaseSmall0) * mul0);
			vertex.v[2] = smallRadius * sinf(phaseSmall0);

			vertices.emplace_back(vertex); // 0, 0

			vertex.t[0] = 1.0f * (j + 1) / smallSectors;
			vertex.t[1] = 1.0f * i / bigSectors;

			vertex.v[0] = cosf(phaseBig0) * (bigRadius + smallRadius * cosf(phaseSmall1) * mul0);
			vertex.v[1] = sinf(phaseBig0) * (bigRadius + smallRadius * cosf(phaseSmall1) * mul0);
			vertex.v[2] = smallRadius * sinf(phaseSmall1);

			vertices.emplace_back(vertex); // 0, 1

			vertex.t[0] = 1.0f * (j + 1) / smallSectors;
			vertex.t[1] = 1.0f * (i + 1) / bigSectors;

			vertex.v[0] = cosf(phaseBig1) * (bigRadius + smallRadius * cosf(phaseSmall1) * mul1);
			vertex.v[1] = sinf(phaseBig1) * (bigRadius + smallRadius * cosf(phaseSmall1) * mul1);
			vertex.v[2] = smallRadius * sinf(phaseSmall1);

			vertices.emplace_back(vertex); // 1, 1


			vertex.t[0] = 1.0f * j / smallSectors;
			vertex.t[1] = 1.0f * i / bigSectors;

			vertex.v[0] = cosf(phaseBig0) * (bigRadius + smallRadius * cosf(phaseSmall0) * mul0);
			vertex.v[1] = sinf(phaseBig0) * (bigRadius + smallRadius * cosf(phaseSmall0) * mul0);
			vertex.v[2] = smallRadius * sinf(phaseSmall0);

			vertices.emplace_back(vertex); // 0, 0

			vertex.t[0] = 1.0f * (j + 1) / smallSectors;
			vertex.t[1] = 1.0f * (i + 1) / bigSectors;

			vertex.v[0] = cosf(phaseBig1) * (bigRadius + smallRadius * cosf(phaseSmall1) * mul1);
			vertex.v[1] = sinf(phaseBig1) * (bigRadius + smallRadius * cosf(phaseSmall1) * mul1);
			vertex.v[2] = smallRadius * sinf(phaseSmall1);

			vertices.emplace_back(vertex); // 1, 1

			vertex.t[0] = 1.0f * j / smallSectors;
			vertex.t[1] = 1.0f * (i + 1) / bigSectors;

			vertex.v[0] = cosf(phaseBig1) * (bigRadius + smallRadius * cosf(phaseSmall0) * mul1);
			vertex.v[1] = sinf(phaseBig1) * (bigRadius + smallRadius * cosf(phaseSmall0) * mul1);
			vertex.v[2] = smallRadius * sinf(phaseSmall0);

			vertices.emplace_back(vertex); // 1, 0

			
		}
	}
#endif

	tri.SetData(vertices.data(), vertices.size() / 3);
 
    return true;
}


void Application::Render()
{
    //float r = (float)rand() / RAND_MAX;
    //float g = (float)rand() / RAND_MAX;
    //float b = (float)rand() / RAND_MAX;

    //glClearColor(r, g, b, 1);
	renderer->BeginRender();
    
	tri.Render();

	renderer->EndRender();
}


void Application::Update(float dt)
{
    static float val = 0.0f;
    static aiMatrix4x4t<float> matRotation;

    aiMatrix4x4t<float> mat2;
#ifdef BUNNY
    float shiftUp = 300.0f;
#else
    float shiftUp = 0;
#endif
    mat2.Translation({ (float)Global::Instance().GetWidth() / 2, (float)Global::Instance().GetHeight() / 2 - shiftUp, 0.0f }, mat2);
    mat = mat2;

    if (Input::MousePressed[0])
    {
        aiVector2D shift(Input::MouseDX, Input::MouseDY);

        float length = shift.Length();

        if (length > 0.01f)
        {
            aiVector3D axis = { shift.y, shift.x, 0.0f };
            axis.Normalize();
            mat2.Rotation(-length * 0.003f, axis, mat2);
            matRotation = mat2 * matRotation;

            //LOG_I(shift.x << " " << shift.y);
        }
    }
    mat *= matRotation;
#ifdef BUNNY
    float coef = 5000.0f;
#else
    float coef = 500.0f;
#endif
    //mat.Translation(val, mat);
    mat2.Scaling({ coef, coef, coef }, mat2);

    mat *= mat2;

    val += 0.01f;

    std::copy(mat[0], mat[0] + 16, tri.matrix);
}



Application::~Application()
{
    
}

// PRIVATE INTERFACE