#include <Windows.h>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class Window
{
public:
    Window();
    Window(const Window&) = delete;
    Window& operator =(const Window&) = delete;
    ~Window();

    bool ProcessMessages();
    void Render(int values[4][4]);
    void RenderMenu();
    void RenderGameOver(int values[4][4]);
    void DrawRect(HDC hdc, int left, int top, int right, int bottom, int r, int g, int b);
    void DrawRoundedRect(HDC hdc, int left, int top, int right, int bottom, int radius, int r, int g, int b);
    void DrawText(HDC hdc, int left, int top, int right, int bottom, const char* text, int fontSize, int r, int g, int b);
    void Get2048Color(int value, int& r, int& g, int& b);
    void GetTextColor(int value, int& r, int& g, int& b);
private:
    HINSTANCE m_hInstance;
    HWND m_hWnd;
};