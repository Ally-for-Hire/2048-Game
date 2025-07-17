#pragma once
#include <Windows.h>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class Window
{
public:
    Window(int width = 400, int height = 400, const char* title = "Window");
    Window(const Window&) = delete;
    Window& operator =(const Window&) = delete;
    ~Window();

    bool ProcessMessages();
    HDC GetDrawContext();
    void ReleaseDrawContext(HDC hdc);
    void DrawRect(HDC hdc, int left, int top, int right, int bottom, int r, int g, int b);
    void DrawRoundedRect(HDC hdc, int left, int top, int right, int bottom, int radius, int r, int g, int b);
    void DrawText(HDC hdc, int left, int top, int right, int bottom, const char* text, int fontSize, int r, int g, int b);

    // Key input handling
    bool IsKeyPressed(int vkCode);
    void ClearKeyPressed(int vkCode);

private:
    HINSTANCE m_hInstance;
    HWND m_hWnd;
    int m_width, m_height;
    bool m_keys[256]; // Track key states
};