#pragma once

class Input
{
public:
    static void Init();
    static void ResetStates();
    static void Update()
    {
        MouseDX = 0;
        MouseDY = 0;
    }
    static void MouseMove(int x, int y)
    {
        MouseDX = x - MouseX;
        MouseDY = y - MouseY;
        MouseX = x;
        MouseY = y;
    }


    static bool MouseDown[3];
    static bool MouseUp[3];
    static bool MousePressed[3];

    static int MouseX;
    static int MouseY;

    static int MouseDX;
    static int MouseDY;
};
