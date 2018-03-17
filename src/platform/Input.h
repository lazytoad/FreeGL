#pragma once

class Input
{
public:
    static void Init();
    static void ResetStates();
    static void MouseMove(int x, int y)
    {
        MouseX = x;
        MouseY = y;
    }
    

    static bool MouseDown[3];
    static bool MouseUp[3];
    static bool MousePressed[3];

    static int MouseX;
    static int MouseY;
};