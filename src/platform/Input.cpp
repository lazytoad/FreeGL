#include "Input.h"

bool Input::MouseDown[3];
bool Input::MouseUp[3];
bool Input::MousePressed[3];

int Input::MouseX;
int Input::MouseY;
int Input::MouseDX;
int Input::MouseDY;

void Input::Init()
{
    for (int i = 0; i < 3; ++i)
    {
        MouseDown[i] = false;
        MouseUp[i] = false;
        MousePressed[i] = false;
    }
    MouseX = -1;
    MouseY = -1;
}

void Input::ResetStates()
{
    Update();
    for (int i = 0; i < 3; ++i)
    {
        MouseDown[i] = false;
        MouseUp[i] = false;
    }
}