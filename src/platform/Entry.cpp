#include <Windows.h>
#include <Windowsx.h>

#include "Consts.h"
#include "QPCTimer.h"
#include "Application.h"
#include "Logger.h"
#include "Renderer.h"
#include "Input.h"

static const wchar_t windowClassName[] = L"FreeGL";
static HWND windowHandle = NULL;
static Application application;

// This ensures render after each update.
// Also helps when long updates give no time to process window messages.
static bool waitForRender = false;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    switch (message)
    {
    case WM_COMMAND:
        wmId = LOWORD(wParam);
        wmEvent = HIWORD(wParam);
        return DefWindowProc(hWnd, message, wParam, lParam);
    case WM_PAINT:
        application.Render();
		waitForRender = false;
        break;
    case WM_MOUSEMOVE:
        {
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);
            Input::MouseMove(xPos, yPos);
        }
        break;
    case WM_LBUTTONDOWN:
        Input::MouseDown[0] = true;
        Input::MousePressed[0] = true;
        break;
    case WM_RBUTTONDOWN:
        Input::MouseDown[1] = true;
        Input::MousePressed[1] = true;
        break;
    case WM_LBUTTONUP:
        Input::MouseUp[0] = true;
        Input::MousePressed[0] = false;
        break;
    case WM_RBUTTONUP:
        Input::MouseUp[1] = true;
        Input::MousePressed[1] = false;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = windowClassName;
    wcex.hIconSm = NULL;

    return RegisterClassEx(&wcex);
}


// The task says, the app should be CONSOLE, so we have this.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow);

int main(int argc, char **argv)
{
    std::string argsJoined = "";
    for (int i = 1; i < argc; ++i)
    {
        argsJoined.append(argv[i]);
        argsJoined.append(" ");
    }
    Logger::InitDefaultLogger("log.log", true);
    LOG_INFO "log initialized" LOG_END;
    // Yes, I use const cast in situations like this. As the string is used as constant and it is easy to change to non-console entry point.
    //return WinMain(NULL, NULL, const_cast<LPSTR>(argsJoined.c_str()), SW_SHOW);

    Input::Init();
    Renderer renderer(nullptr);
    renderer.Initialize();

    application.Init(&renderer, argsJoined.c_str());

    MSG msg;
    /*PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
    TranslateMessage(&msg);
    DispatchMessage(&msg);*/

    double time = 0;

    QPCTimer timer;
    int feets = 0;

    while (true)
    {
        timer.Start();
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            //LOG_E(msg.message);
            if (msg.message == WM_QUIT)
            {
                return 0;
            }
            else
            {
                switch (msg.message)
                {
                    case WM_MOUSEMOVE:
                    {
                        int xPos = GET_X_LPARAM(msg.lParam);
                        int yPos = GET_Y_LPARAM(msg.lParam);
                        Input::MouseMove(xPos, yPos);

						//LOG_INFO "X: " << xPos << " " << yPos LOG_END;
                    }
                    break;
                    case WM_LBUTTONDOWN:
                        Input::MouseDown[0] = true;
                        Input::MousePressed[0] = true;
                        break;
                    case WM_RBUTTONDOWN:
                        Input::MouseDown[1] = true;
                        Input::MousePressed[1] = true;
                        break;
                    case WM_LBUTTONUP:
                        Input::MouseUp[0] = true;
                        Input::MousePressed[0] = false;
                        break;
                    case WM_RBUTTONUP:
                        Input::MouseUp[1] = true;
                        Input::MousePressed[1] = false;
                        break;
                }
            }
        }
        //else
        {
            application.Render();

            //LOG_E("X= " << Input::MouseX << " Y= " << Input::MouseY << " pressed= " << Input::MousePressed[0]);
            Input::ResetStates();
            //renderer.BeginRender();
            //renderer.EndRender();
        }
        double t = timer.Stamp();
        time += t;
        ++feets;

        if (time >= 1)
        {
            LOG_E("fps = " << (feets / time));
            time = 0;
            feets = 0;
        }
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
    

    MyRegisterClass(hInstance);

    MSG msg;

    windowHandle = CreateWindow(windowClassName, L"FGL", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, Consts::DefaultSceneWidth, Consts::DefaultSceneHeight, NULL, NULL, hInstance, NULL);

	// This actually could fail because of the security policies. But I think, honouring them here will make the code worse with no real benefits.
    

    if (!windowHandle)
    {
        LOG_ERROR "failed to create window!" LOG_END;
        return FALSE;
    }

    Renderer renderer(windowHandle);


    if (!application.Init(&renderer, pCmdLine))
    {
        return -2;
    }

    ShowWindow(windowHandle, nCmdShow);
    UpdateWindow(windowHandle);

    const double frameTargetTime = 1.0 / Consts::TargetFPS;

    QPCTimer mainLoopTimer;
    
    mainLoopTimer.Start();

    while (true)
    {
        BOOL hadMessage = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
        if (hadMessage)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
            {
                LOG_INFO "quit" LOG_END
                return 0;
            }
        }
        double frameTime = mainLoopTimer.Stamp();
        if (frameTime >= frameTargetTime && !waitForRender)
        {
            if (frameTime > 0 && frameTime < 0.3)
            {
                bool brk = true;
            }
            mainLoopTimer.Start();
            application.Update((float)frameTime);
            InvalidateRect(windowHandle, NULL, false);
			waitForRender = true;
        }
        else if (!hadMessage && frameTargetTime - frameTime >= 0.005) 
        {// As Windows' Sleep() really has like +-10 msec precision, we cannot really sleep for 1 msec.
            Sleep(1);
        }
    }
    return 0;
}