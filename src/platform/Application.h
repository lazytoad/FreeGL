#pragma once

#include <Windows.h>
#include <vector>

class Renderer;


struct Point2f
{
    float x;
    float y;
};

class Application
{
    public:
        bool Init(Renderer *renderer, const char * commandLine);
        void Render();
        void Update(float dt);
        ~Application();

    private:
		void forceFinishPhysics();

        Renderer * renderer;

        Point2f ViewPos;

        float DTAccumulated;

        std::vector<Point2f>     Path;
};