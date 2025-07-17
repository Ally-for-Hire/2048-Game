#include "Window.h"
#include <iostream>

int main() {
    std::cout << "Creating Window\n";
    
    Window* pWindow = new Window();

    bool running = true;
    while (running)
    {
        if (!pWindow->ProcessMessages())
        {
            std::cout << "Closing Window\n";
            running = false;
        }
        
        // Render
        Sleep(10);
    }

    delete pWindow;
    
    return 0;
}