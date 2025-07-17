#include "Window.h"
#include <iostream>
#include <cstdio>
#include <cstring>

// Global key state for the window
static bool g_keys[256] = {false};

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
        }
        return 0;
    case WM_KEYDOWN:
        if (wParam < 256) {
            g_keys[wParam] = true;
        }
        return 0;
    case WM_KEYUP:
        if (wParam < 256) {
            g_keys[wParam] = false;
        }
        return 0;
    }

    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

Window::Window(int width, int height, const char* title)
    : m_hInstance(GetModuleHandle(nullptr)), m_width(width), m_height(height)
{
    // Initialize key states
    for (int i = 0; i < 256; i++) {
        m_keys[i] = false;
    }
    
    const wchar_t* CLASS_NAME = L"Window Class";

    WNDCLASSW wndClass = {};
    wndClass.lpszClassName = CLASS_NAME;
    wndClass.hInstance = m_hInstance;
    wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.lpfnWndProc = WindowProc;

    RegisterClassW(&wndClass);

    DWORD style = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;

    RECT rect;
    rect.left = 250;
    rect.top = 250;
    rect.right = rect.left + width;
    rect.bottom = rect.top + height;

    AdjustWindowRect(&rect, style, false);

    // Convert title to wide string
    int titleLen = strlen(title) + 1;
    wchar_t* wTitle = new wchar_t[titleLen];
    MultiByteToWideChar(CP_UTF8, 0, title, -1, wTitle, titleLen);

    m_hWnd = CreateWindowExW(
        0,
        CLASS_NAME,
        wTitle,
        style,
        rect.left,
        rect.top,
        rect.right - rect.left,
        rect.bottom - rect.top,
        NULL,
        NULL,
        m_hInstance,
        NULL
    );

    delete[] wTitle;
    ShowWindow(m_hWnd, SW_SHOW);
}

Window::~Window()
{
    const wchar_t* CLASS_NAME = L"Window Class";
    UnregisterClassW(CLASS_NAME, m_hInstance);
}

bool Window::ProcessMessages()
{
    MSG msg = {};

    while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            return false;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return true;
}

HDC Window::GetDrawContext()
{
    return GetDC(m_hWnd);
}

void Window::ReleaseDrawContext(HDC hdc)
{
    ReleaseDC(m_hWnd, hdc);
}

bool Window::IsKeyPressed(int vkCode)
{
    if (vkCode < 0 || vkCode >= 256) return false;
    return g_keys[vkCode];
}

void Window::ClearKeyPressed(int vkCode)
{
    if (vkCode >= 0 && vkCode < 256) {
        g_keys[vkCode] = false;
    }
}

void Window::DrawRect(HDC hdc, int left, int top, int right, int bottom, int r, int g, int b)
{
    HBRUSH brush = CreateSolidBrush(RGB(r, g, b));
    HPEN nullPen = (HPEN)GetStockObject(NULL_PEN);
    
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);
    HPEN oldPen = (HPEN)SelectObject(hdc, nullPen);
    
    Rectangle(hdc, left, top, right, bottom);
    
    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(brush);
}

void Window::DrawRoundedRect(HDC hdc, int left, int top, int right, int bottom, int radius, int r, int g, int b)
{
    HBRUSH brush = CreateSolidBrush(RGB(r, g, b));
    HPEN nullPen = (HPEN)GetStockObject(NULL_PEN);
    
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);
    HPEN oldPen = (HPEN)SelectObject(hdc, nullPen);
    
    RoundRect(hdc, left, top, right, bottom, radius * 2, radius * 2);
    
    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(brush);
}

void Window::DrawText(HDC hdc, int left, int top, int right, int bottom, const char* text, int fontSize, int r, int g, int b)
{
    // Create font with specified size
    HFONT font = CreateFontA(
        fontSize,                   // Height
        0,                          // Width (0 = default)
        0,                          // Escapement
        0,                          // Orientation
        FW_BOLD,                    // Weight
        FALSE,                      // Italic
        FALSE,                      // Underline
        FALSE,                      // StrikeOut
        DEFAULT_CHARSET,            // CharSet
        OUT_DEFAULT_PRECIS,         // OutPrecision
        CLIP_DEFAULT_PRECIS,        // ClipPrecision
        DEFAULT_QUALITY,            // Quality
        DEFAULT_PITCH | FF_SWISS,   // PitchAndFamily
        "Arial"                     // Font name
    );
    
    // Select font and set text properties
    HFONT oldFont = (HFONT)SelectObject(hdc, font);
    SetTextColor(hdc, RGB(r, g, b));
    SetBkMode(hdc, TRANSPARENT);
    
    // Calculate center position
    int centerX = (left + right) / 2;
    int centerY = (top + bottom) / 2;
    
    // Get text dimensions for centering
    SIZE textSize;
    GetTextExtentPoint32A(hdc, text, strlen(text), &textSize);
    
    // Center the text
    int textX = centerX - textSize.cx / 2;
    int textY = centerY - textSize.cy / 2;
    
    // Draw the text
    TextOutA(hdc, textX, textY, text, strlen(text));
    
    // Clean up
    SelectObject(hdc, oldFont);
    DeleteObject(font);
}
