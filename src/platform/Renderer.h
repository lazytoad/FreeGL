#pragma once

#include <Windows.h>
#include <string>

#include "Consts.h"

#include "RenderCore.h"
#include "Program.h"

class Renderer
{
    public:
        struct WindowCreationParams
        {
            RECT rect;
            std::wstring windowName;
            bool fullscreen;
            bool resizable;
            int samples;
        };

        Renderer(HWND hwnd, float sceneWidth = Consts::DefaultSceneWidth, float sceneHeight = Consts::DefaultSceneHeight);
        ~Renderer();

        bool Initialize();

        void BeginRender();
        void EndRender();

        void SetSceneSize(float sceneWidth, float sceneHeight);

    private:
        bool createWindow(WindowCreationParams* params, HWND* hwnd, HDC* hdc);
        bool initializeGL(WindowCreationParams* params);
        void setupTransform();

        PAINTSTRUCT ps;
        HWND        hwnd;
        HDC         hdc;
        HDC         dcBuffer;
        HPEN        hpen;
        HBITMAP     bitmap;
        RECT        clientRect;
        RECT        bmpRect;

        float tx;
        float ty;
        float scalex;
        float scaley;

        float sceneWidth;
        float sceneHeight;

        bool isReady;

        unsigned vertexBuffer;
        unsigned vertexArray;
        unsigned vertexShader;
        unsigned fragmentShader;
        unsigned shaderProgram;

        unsigned zBuffer;
        Program clearProgram;

        RenderCore renderCore;
};