#include "Window.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>

// Global variables for key communication
extern bool keyPressed;
extern int lastKey;
extern bool needsRedraw;

// Game state
enum GameState {
    MENU,
    PLAYING,
    GAME_OVER
};
extern GameState currentState;

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
        {
            switch (wParam)
            {
            case VK_UP:
            case VK_DOWN:
            case VK_LEFT:
            case VK_RIGHT:
            case VK_RETURN:  // Enter key
                keyPressed = true;
                lastKey = wParam;
                needsRedraw = true;
                std::cout << "Key pressed: " << wParam << "\n";
                break;
            }
        }
        return 0;
    }

    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

Window::Window()
    : m_hInstance(GetModuleHandle(nullptr))
{
    const wchar_t* CLASS_NAME = L"Window Class";

    WNDCLASSW wndClass = {};
    wndClass.lpszClassName = CLASS_NAME;
    wndClass.hInstance = m_hInstance;
    wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.lpfnWndProc = WindowProc;

    RegisterClassW(&wndClass);

    DWORD style = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;

    int height = 395;
    int width = 395;

    RECT rect;
    rect.left = 250;
    rect.top = 250;
    rect.right = rect.left + width;
    rect.bottom = rect.top + height;

    AdjustWindowRect(&rect, style, false);

    m_hWnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"2048 Game",
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

    ShowWindow(m_hWnd, SW_SHOW);

    // Store the Window instance pointer in the window's user data
    SetWindowLongPtr(m_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    // Initialize random seed once
    srand(time(NULL));
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

void Window::Render(int values[4][4])
{
    HDC hdc = GetDC(m_hWnd);

    DrawRoundedRect(hdc, 0, 0, 400, 400, 2, 70, 65, 50);

    int spacing = 10;
    int cellSize = (400 - (spacing * 5)) / 4;

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            int x = spacing + col * (cellSize + spacing);
            int y = spacing + row * (cellSize + spacing);

            if (values[row][col] != 0) {  // Only draw non-zero values
                // Get 2048 color scheme
                int r, g, b;
                Get2048Color(values[row][col], r, g, b);
                DrawRoundedRect(hdc, x, y, x + cellSize, y + cellSize, 2, r, g, b);

                char text[16];
                sprintf(text, "%d", values[row][col]);

                // Get text color (dark for light tiles, light for dark tiles)
                int textR, textG, textB;
                GetTextColor(values[row][col], textR, textG, textB);
                DrawText(hdc, x, y, x + cellSize, y + cellSize, text, 36, textR, textG, textB);
            }
            else
            {
                DrawRoundedRect(hdc, x, y, x + cellSize, y + cellSize, 2, 140, 130, 100);  // Empty cell color
            }
        }
    }

    // Release device context
    ReleaseDC(m_hWnd, hdc);
}

void Window::RenderMenu()
{
    HDC hdc = GetDC(m_hWnd);

    // Draw black background
    DrawRoundedRect(hdc, 0, 0, 400, 400, 2, 0, 0, 0);

    // Draw title "2048"
    DrawText(hdc, 0, 80, 400, 140, "2048", 48, 255, 255, 255);

    // Draw instructions
    DrawText(hdc, 0, 200, 400, 240, "Press [ENTER] to Start", 24, 200, 200, 200);

    ReleaseDC(m_hWnd, hdc);
}

void Window::RenderGameOver(int values[4][4])
{
    // First render the game board
    Render(values);

    // Then draw overlay
    HDC hdc = GetDC(m_hWnd);

    // Draw semi-transparent overlay
    DrawRoundedRect(hdc, 50, 150, 350, 250, 5, 0, 0, 0);

    // Draw game over text
    DrawText(hdc, 50, 160, 350, 200, "You Lost!", 32, 255, 100, 100);
    DrawText(hdc, 50, 210, 350, 240, "Press [ENTER] to try again!", 18, 200, 200, 200);

    ReleaseDC(m_hWnd, hdc);
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
        fontSize,                    // Height
        0,                          // Width (0 = default)
        0,                          // Escapement
        0,                          // Orientation
        FW_BOLD,                  // Weight
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

void Window::Get2048Color(int value, int& r, int& g, int& b)
{
    switch (value) {
        case 2:    r = 238; g = 228; b = 218; break;  // Light beige
        case 4:    r = 237; g = 224; b = 200; break;  // Slightly darker beige
        case 8:    r = 242; g = 177; b = 121; break;  // Light orange
        case 16:   r = 245; g = 149; b = 99;  break;  // Orange
        case 32:   r = 246; g = 124; b = 95;  break;  // Red-orange
        case 64:   r = 246; g = 94;  b = 59;  break;  // Red
        case 128:  r = 237; g = 207; b = 114; break;  // Light yellow
        case 256:  r = 237; g = 204; b = 97;  break;  // Yellow
        case 512:  r = 237; g = 200; b = 80;  break;  // Gold
        case 1024: r = 237; g = 197; b = 63;  break;  // Dark gold
        case 2048: r = 237; g = 194; b = 46;  break;  // Bright gold
        default:   r = 60;  g = 58;  b = 50;  break;  // Dark for higher values
    }
}

void Window::GetTextColor(int value, int& r, int& g, int& b)
{
    if (value <= 4) {
        // Dark text for light tiles
        r = 119; g = 110; b = 101;
    } else {
        // White text for darker tiles
        r = 249; g = 246; b = 242;
    }
}