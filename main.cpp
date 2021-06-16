#ifndef UNICODE
#define UNICODE
#endif // always use unicode in windows

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <glad/glad.h>
#include<cstdio>

static bool isOpen;
const wchar_t g_szClassName[] = L"BareMetalKlass";
HDC dc;
HGLRC rc;

void LoadGL(HWND hwnd,HDC hdc,HGLRC rc,LPARAM lparam) {
    PIXELFORMATDESCRIPTOR pfd = {
            sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd
            1,                     // version number
            PFD_DRAW_TO_WINDOW |   // support window
            PFD_SUPPORT_OPENGL |   // support OpenGL
            PFD_DOUBLEBUFFER,      // double buffered
            PFD_TYPE_RGBA,         // RGBA type
            24,                    // 24-bit color depth
            0, 0, 0, 0, 0, 0,      // color bits ignored
            0,                     // no alpha buffer
            0,                     // shift bit ignored
            0,                     // no accumulation buffer
            0, 0, 0, 0,            // accum bits ignored
            32,                    // 32-bit z-buffer
            0,                     // no stencil buffer
            0,                     // no auxiliary buffer
            PFD_MAIN_PLANE,        // main layer
            0,                     // reserved
            0, 0, 0                // layer masks ignored
    };
    dc = GetDC(hwnd);
    int pf = ChoosePixelFormat(dc, &pfd);
    SetPixelFormat(dc, pf, &pfd);
    rc = wglCreateContext(dc);
    wglMakeCurrent(dc, rc);
    gladLoadGL();
    int width = LOWORD(lparam);
    int height = HIWORD(lparam);
    glViewport(0,0,width,height);
    printf("GL Version: %d.%d\n", GLVersion.major,GLVersion.minor);
}

void GLPaint(HDC dc) {
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    SwapBuffers(dc);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_CREATE :{
            LoadGL(hwnd,dc,rc,lParam);
        }break;
        case WM_SIZE : {
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);
            glViewport(0,0,width,height);
        }break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            isOpen = false;
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int main(int argc,char *argv[])
{

    HINSTANCE  hInstance = GetModuleHandle(nullptr);
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;
    STARTUPINFO si;
    GetStartupInfo(&si);
    int nCmdShow = si.wShowWindow;

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = CS_OWNDC;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;//(HBRUSH) CreateSolidBrush(RGB(20, 20, 30));
    wc.lpszMenuName  = nullptr;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(nullptr, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
    {
        MessageBox(nullptr, L"Window Registration Failed!", L"Error!",
                   MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    hwnd = CreateWindowEx(
            WS_EX_CLIENTEDGE,
            g_szClassName,
            L"BareMetal",
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT, CW_USEDEFAULT, 700, 700,
            nullptr, nullptr, hInstance, nullptr);

    if(hwnd == nullptr)
    {
        MessageBox(nullptr, L"Window Creation Failed!", L"Error!",
                   MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    isOpen = true;

    while(isOpen) {
        while (PeekMessage(&Msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&Msg);
            DispatchMessage(&Msg);
        }
        GLPaint(dc);
    }
    return Msg.wParam;
}