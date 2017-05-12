#include "Renderer.h"
#include "Logger.h"

#include "GL/glew.h"
#include "GL/wglew.h"

#include <assert.h>
#include <string>



#define DEFAULT_COLOR_BUFFER_SIZE 32
#define DEFAULT_DEPTH_BUFFER_SIZE 24
#define DEFAULT_STENCIL_BUFFER_SIZE 8

static HGLRC __hrc = 0; // \todo move shit to Entry
static HINSTANCE __hinstance = 0;

static bool vsync = true;

static bool multiSampling = false;

struct Config
{
    Config()
    {
        windowTitle = "FreeGL";
        isFullscreen = false;
        isResizeable = false;

        x = -1;
        y = -1;
        width = 800;
        height = 600;

        samples = 0;
    }

    std::string windowTitle;
    bool isFullscreen;
    bool isResizeable;

    int x;
    int y;
    int width;
    int height;
    int samples;
};

static Config config;

bool Renderer::createWindow(WindowCreationParams* params, HWND* hwnd, HDC* hdc)
{
	bool fullscreen = false;
	bool resizable = false;
	RECT rect = { CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT };
	std::wstring windowName;
	if (params)
	{
		windowName = params->windowName;
		memcpy(&rect, &params->rect, sizeof(RECT));
		fullscreen = params->fullscreen;
		resizable = params->resizable;
	}

	// Set the window style.
	DWORD style, styleEx;
	if (fullscreen)
	{
		style = WS_POPUP;
		styleEx = WS_EX_APPWINDOW;
	}
	else
	{
		if (resizable)
			style = WS_OVERLAPPEDWINDOW;
		else
			style = WS_POPUP | WS_BORDER | WS_CAPTION | WS_SYSMENU;
		styleEx = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	}
	style |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	// Adjust the window rectangle so the client size is the requested size.
	AdjustWindowRectEx(&rect, style, FALSE, styleEx);

	// Create the native Windows window.
	*hwnd = CreateWindowEx(styleEx, L"freegl", windowName.c_str(), style, 0, 0, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, ::GetModuleHandle(NULL), NULL);
	if (*hwnd == NULL)
	{
		LOG_E("Failed to create window.");
		return false;
	}

	// Get the drawing context.
	*hdc = GetDC(*hwnd);
	if (*hdc == NULL)
	{
        LOG_E("Failed to get device context.");
		return false;
	}

	// Center the window
	GetWindowRect(*hwnd, &rect);
	const int screenX = (GetSystemMetrics(SM_CXSCREEN) - rect.right) / 2;
	const int screenY = (GetSystemMetrics(SM_CYSCREEN) - rect.bottom) / 2;
	SetWindowPos(*hwnd, *hwnd, screenX, screenY, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

	return true;
}

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    switch (message)
    {
    case WM_COMMAND:
        wmId = LOWORD(wParam);
        wmEvent = HIWORD(wParam);
        return DefWindowProc(hWnd, message, wParam, lParam);
    //case WM_PAINT:
        //application.Render();
        //waitForRender = false;
    //    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

bool Renderer::Initialize()
{
    WindowCreationParams params;
    params.fullscreen = false;
    params.resizable = false;
    params.rect.left = 0;
    params.rect.top = 0;
    params.rect.right = 0;
    params.rect.bottom = 0;
    params.samples = 0;

    int len = MultiByteToWideChar(CP_ACP, 0, config.windowTitle.c_str(), -1, NULL, 0);
    wchar_t* wtitle = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, config.windowTitle.c_str(), -1, wtitle, len);
    params.windowName = wtitle;
    delete[] wtitle;

    params.fullscreen = config.isFullscreen;
    // Read resizable state.
    params.resizable = config.isResizeable;
    // Read multisampling state.
    params.samples = config.samples;

    if (config.x < 0) // \todo cleanup, add getting display resolution
    {
        config.x = 40;
    }
    if (config.y < 0)
    {
        config.y = 20;
    }

    params.rect.left = config.x;
    params.rect.right = config.x + config.width;
    params.rect.top = config.y;
    params.rect.bottom = config.y + config.height;

    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = (WNDPROC)WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = __hinstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;  // No brush - we are going to paint our own background
    wc.lpszMenuName = NULL;  // No default menu
    wc.lpszClassName = L"freegl";

    if (!::RegisterClassEx(&wc))
    {
        return false;
    }

    if (initializeGL(&params))
    {
        ShowWindow(hwnd, SW_SHOW);
        return true;
    }
    else
        return false;
}

bool Renderer::initializeGL(WindowCreationParams* params)
{
    // Create a temporary window and context to we can initialize GLEW and get access
    // to additional OpenGL extension functions. This is a neccessary evil since the
    // function for querying GL extensions is a GL extension itself.
    hwnd = NULL;
    hdc = NULL;

    if (params)
    {
        if (!createWindow(params, &hwnd, &hdc))
            return false;
    }
    else
    {
        //hwnd = __hwnd;
        //hdc = __hdc;
    }

    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = DEFAULT_COLOR_BUFFER_SIZE;
    pfd.cDepthBits = DEFAULT_DEPTH_BUFFER_SIZE;
    pfd.cStencilBits = DEFAULT_STENCIL_BUFFER_SIZE;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pixelFormat = ChoosePixelFormat(hdc, &pfd);
    if (pixelFormat == 0)
    {
        DestroyWindow(hwnd);
        LOG_E("Failed to choose a pixel format.");
        return false;
    }

    if (!SetPixelFormat(hdc, pixelFormat, &pfd))
    {
        DestroyWindow(hwnd);
        LOG_E("Failed to set the pixel format.");
        return false;
    }

    HGLRC tempContext = wglCreateContext(hdc);
    if (!tempContext)
    {
        DestroyWindow(hwnd);
        LOG_E("Failed to create temporary context for initialization.");
        return false;
    }
    wglMakeCurrent(hdc, tempContext);

    // Initialize GLEW
    if (GLEW_OK != glewInit())
    {
        wglDeleteContext(tempContext);
        DestroyWindow(hwnd);
        LOG_E("Failed to initialize GLEW.");
        return false;
    }
    
	if (wglChoosePixelFormatARB && wglCreateContextAttribsARB)
    {
        // Choose pixel format using wglChoosePixelFormatARB, which allows us to specify
        // additional attributes such as multisampling.
        //
        // Note: Keep multisampling attributes at the start of the attribute lists since code below
        // assumes they are array elements 0 through 3.
        int attribList[] = {
            WGL_SAMPLES_ARB, params ? params->samples : 0,
            WGL_SAMPLE_BUFFERS_ARB, params ? (params->samples > 0 ? 1 : 0) : 0,
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
            WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
            WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
            WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
            WGL_COLOR_BITS_ARB, DEFAULT_COLOR_BUFFER_SIZE,
            WGL_DEPTH_BITS_ARB, DEFAULT_DEPTH_BUFFER_SIZE,
            WGL_STENCIL_BITS_ARB, DEFAULT_STENCIL_BUFFER_SIZE,
            0
        };
        multiSampling = params && params->samples > 0;

		UINT numFormats;
        if (!wglChoosePixelFormatARB(hdc, attribList, NULL, 1, &pixelFormat, &numFormats) || numFormats == 0)
        {
            bool valid = false;
            if (params && params->samples > 0)
            {
                LOG_WARNING "Failed to choose pixel format with WGL_SAMPLES_ARB == " << params->samples << ". Attempting to fallback to lower samples setting." LOG_END;
                while (params->samples > 0)
                {
                    params->samples /= 2;
                    attribList[1] = params->samples;
                    attribList[3] = params->samples > 0 ? 1 : 0;
                    if (wglChoosePixelFormatARB(hdc, attribList, NULL, 1, &pixelFormat, &numFormats) && numFormats > 0)
                    {
                        valid = true;
                        LOG_WARNING "Found pixel format with WGL_SAMPLES_ARB == " << params->samples LOG_END;
                        break;
                    }
                }

                multiSampling = params->samples > 0;
            }

            if (!valid)
            {
                wglDeleteContext(tempContext);
                DestroyWindow(hwnd);
                LOG_E("Failed to choose a pixel format.");
                return false;
            }
        }
		
        // Create new/final window if needed
        if (params)
        {
            DestroyWindow(hwnd);
            
            MSG msg;
            while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            hwnd = NULL;
            hdc = NULL;

            if (!createWindow(params, &hwnd, &hdc))
            {
                wglDeleteContext(tempContext);
                return false;
            }
        }
		
        // Set final pixel format for window
        if (!SetPixelFormat(hdc, pixelFormat, &pfd))
        {
            LOG_E("Failed to set the pixel format: %d." << (int)GetLastError());
            return false;
        }

        // Create our new GL context
        int attribs[] =
        {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
            WGL_CONTEXT_MINOR_VERSION_ARB, 0,
            0
        };

        if (!(__hrc = wglCreateContextAttribsARB(hdc, 0, attribs)))
        {
            wglDeleteContext(tempContext);
            LOG_E("Failed to create OpenGL context.");
            return false;
        }

        // Delete the old/temporary context and window
        wglDeleteContext(tempContext);

        // Make the new context current
        if (!wglMakeCurrent(hdc, __hrc) || !__hrc)
        {
            LOG_E("Failed to make the window current.");
            return false;
        }
    }
    else
    {
        LOG_E("Failed to initialize OpenGL.");
        assert(false);
    }
	/*else    // fallback to OpenGL 2.0 if wglChoosePixelFormatARB or wglCreateContextAttribsARB is NULL.
    {
        // Context is already here, just use it.
        __hrc = tempContext;
        __hwnd = hwnd;
        __hdc = hdc;
    }*/
	
    // Vertical sync.
    if (wglSwapIntervalEXT)
        wglSwapIntervalEXT(vsync ? 1 : 0);
    else
        vsync = false;
		

    // Some old graphics cards support EXT_framebuffer_object instead of ARB_framebuffer_object.
    // Patch ARB_framebuffer_object functions to EXT_framebuffer_object ones since semantic is same.
    if (!GLEW_ARB_framebuffer_object && GLEW_EXT_framebuffer_object)
    {
        glBindFramebuffer = glBindFramebufferEXT;
        glBindRenderbuffer = glBindRenderbufferEXT;
        glBlitFramebuffer = glBlitFramebufferEXT;
        glCheckFramebufferStatus = glCheckFramebufferStatusEXT;
        glDeleteFramebuffers = glDeleteFramebuffersEXT;
        glDeleteRenderbuffers = glDeleteRenderbuffersEXT;
        glFramebufferRenderbuffer = glFramebufferRenderbufferEXT;
        glFramebufferTexture1D = glFramebufferTexture1DEXT;
        glFramebufferTexture2D = glFramebufferTexture2DEXT;
        glFramebufferTexture3D = glFramebufferTexture3DEXT;
        glFramebufferTextureLayer = glFramebufferTextureLayerEXT;
        glGenFramebuffers = glGenFramebuffersEXT;
        glGenRenderbuffers = glGenRenderbuffersEXT;
        glGenerateMipmap = glGenerateMipmapEXT;
        glGetFramebufferAttachmentParameteriv = glGetFramebufferAttachmentParameterivEXT;
        glGetRenderbufferParameteriv = glGetRenderbufferParameterivEXT;
        glIsFramebuffer = glIsFramebufferEXT;
        glIsRenderbuffer = glIsRenderbufferEXT;
        glRenderbufferStorage = glRenderbufferStorageEXT;
        glRenderbufferStorageMultisample = glRenderbufferStorageMultisampleEXT;
    }

    glClearColor(0, 1, 0.2f, 1);
    
    return true;
}

Renderer::Renderer(HWND _hwnd, float sceneWidth, float sceneHeight) : hwnd(_hwnd), hpen(NULL), bitmap(NULL), dcBuffer(NULL),
sceneWidth(sceneWidth), sceneHeight(sceneHeight)
{
	//hdc = GetDC(hwnd);
	//HGLRC tempContext = wglCreateContext(hdc);
	//dcBuffer = CreateCompatibleDC(hdc);
}

Renderer::~Renderer()
{
	DeleteDC(dcBuffer);
    DeleteObject(bitmap);
}


void Renderer::BeginRender()
{
    //hdc = BeginPaint(hwnd, &ps);
    
    /*setupTransform();
    if (bitmap == NULL || bmpRect.right != clientRect.right || bmpRect.bottom != clientRect.bottom )
    {
        DeleteObject(bitmap);
        bitmap = CreateCompatibleBitmap(dcBuffer, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
        bmpRect = clientRect;
    }
    
    SelectObject(dcBuffer, bitmap);
    FillRect(dcBuffer, &clientRect, (HBRUSH)(COLOR_WINDOW+1));
    
    hpen = CreatePen(PS_SOLID, 1, 0xff000000);*/

    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::EndRender()
{
    /*DeleteObject(hpen);
    
    BitBlt(hdc, clientRect.left, clientRect.top, clientRect.right, clientRect.bottom, dcBuffer, 0, 0, SRCCOPY);
    
    EndPaint(hwnd, &ps);*/
    SwapBuffers(hdc);
}

void Renderer::SetSceneSize(float sceneWidth, float sceneHeight)
{
    sceneWidth = sceneWidth;
    sceneHeight = sceneHeight;
}


void Renderer::setupTransform()
{
    GetClientRect(hwnd, &clientRect);

    tx = (clientRect.right - clientRect.left) * 0.1f;
    ty = (clientRect.bottom - clientRect.top) * 0.9f;

    scalex = (clientRect.right - clientRect.left) * 0.8f / sceneWidth;
    scaley = -(clientRect.bottom - clientRect.top) * 0.8f / sceneHeight;
}