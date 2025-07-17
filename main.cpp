#include "Window.h"
#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <ctime>

// 2048 Game Logic
class Game2048 {
public:
    Game2048() {
        srand(time(NULL));
        InitializeGrid();
        AddRandomTile();
        AddRandomTile();
    }
    
    void InitializeGrid() {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                grid[i][j] = 0;
            }
        }
    }
    
    void AddRandomTile() {
        // Find empty cells
        int emptyCells[16][2];
        int emptyCount = 0;
        
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                if (grid[row][col] == 0) {
                    emptyCells[emptyCount][0] = row;
                    emptyCells[emptyCount][1] = col;
                    emptyCount++;
                }
            }
        }
        
        if (emptyCount > 0) {
            int randomIndex = rand() % emptyCount;
            int row = emptyCells[randomIndex][0];
            int col = emptyCells[randomIndex][1];
            
            int roll = rand() % 100 + 1;
            int value = (roll >= 90) ? 4 : 2;  // 10% chance for 4, 90% for 2
            
            grid[row][col] = value;
        }
    }
    
    bool MoveLeft() {
        bool moved = false;
        for (int row = 0; row < 4; row++) {
            int writePos = 0;
            bool merged[4] = {false, false, false, false};
            
            for (int col = 0; col < 4; col++) {
                if (grid[row][col] != 0) {
                    if (writePos > 0 && grid[row][writePos-1] == grid[row][col] && !merged[writePos-1]) {
                        grid[row][writePos-1] *= 2;
                        merged[writePos-1] = true;
                        grid[row][col] = 0;
                        moved = true;
                    } else {
                        if (writePos != col) {
                            grid[row][writePos] = grid[row][col];
                            grid[row][col] = 0;
                            moved = true;
                        }
                        writePos++;
                    }
                }
            }
        }
        return moved;
    }
    
    bool MoveRight() {
        bool moved = false;
        for (int row = 0; row < 4; row++) {
            int writePos = 3;
            bool merged[4] = {false, false, false, false};
            
            for (int col = 3; col >= 0; col--) {
                if (grid[row][col] != 0) {
                    if (writePos < 3 && grid[row][writePos+1] == grid[row][col] && !merged[writePos+1]) {
                        grid[row][writePos+1] *= 2;
                        merged[writePos+1] = true;
                        grid[row][col] = 0;
                        moved = true;
                    } else {
                        if (writePos != col) {
                            grid[row][writePos] = grid[row][col];
                            grid[row][col] = 0;
                            moved = true;
                        }
                        writePos--;
                    }
                }
            }
        }
        return moved;
    }
    
    bool MoveUp() {
        bool moved = false;
        for (int col = 0; col < 4; col++) {
            int writePos = 0;
            bool merged[4] = {false, false, false, false};
            
            for (int row = 0; row < 4; row++) {
                if (grid[row][col] != 0) {
                    if (writePos > 0 && grid[writePos-1][col] == grid[row][col] && !merged[writePos-1]) {
                        grid[writePos-1][col] *= 2;
                        merged[writePos-1] = true;
                        grid[row][col] = 0;
                        moved = true;
                    } else {
                        if (writePos != row) {
                            grid[writePos][col] = grid[row][col];
                            grid[row][col] = 0;
                            moved = true;
                        }
                        writePos++;
                    }
                }
            }
        }
        return moved;
    }
    
    bool MoveDown() {
        bool moved = false;
        for (int col = 0; col < 4; col++) {
            int writePos = 3;
            bool merged[4] = {false, false, false, false};
            
            for (int row = 3; row >= 0; row--) {
                if (grid[row][col] != 0) {
                    if (writePos < 3 && grid[writePos+1][col] == grid[row][col] && !merged[writePos+1]) {
                        grid[writePos+1][col] *= 2;
                        merged[writePos+1] = true;
                        grid[row][col] = 0;
                        moved = true;
                    } else {
                        if (writePos != row) {
                            grid[writePos][col] = grid[row][col];
                            grid[row][col] = 0;
                            moved = true;
                        }
                        writePos--;
                    }
                }
            }
        }
        return moved;
    }
    
    bool HasPossibleMoves() {
        // Check for empty cells
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (grid[i][j] == 0) {
                    return true;
                }
            }
        }
        
        // Check for possible merges
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if ((j < 3 && grid[i][j] == grid[i][j+1]) ||
                    (i < 3 && grid[i][j] == grid[i+1][j])) {
                    return true;
                }
            }
        }
        
        return false;
    }
    
    void Get2048Color(int value, int& r, int& g, int& b) {
        switch (value) {
            case 2:    r = 238; g = 228; b = 218; break;
            case 4:    r = 237; g = 224; b = 200; break;
            case 8:    r = 242; g = 177; b = 121; break;
            case 16:   r = 245; g = 149; b = 99;  break;
            case 32:   r = 246; g = 124; b = 95;  break;
            case 64:   r = 246; g = 94;  b = 59;  break;
            case 128:  r = 237; g = 207; b = 114; break;
            case 256:  r = 237; g = 204; b = 97;  break;
            case 512:  r = 237; g = 200; b = 80;  break;
            case 1024: r = 237; g = 197; b = 63;  break;
            case 2048: r = 237; g = 194; b = 46;  break;
            default:   r = 60;  g = 58;  b = 50;  break;
        }
    }
    
    void GetTextColor(int value, int& r, int& g, int& b) {
        if (value <= 4) {
            r = 119; g = 110; b = 101;  // Dark text
        } else {
            r = 249; g = 246; b = 242;  // White text
        }
    }
    
    int grid[4][4];
};

enum GameState { MENU, PLAYING, GAME_OVER };

int main() {
    std::cout << "Creating 2048 Game Window\n";
    
    Window window(400, 400, "2048 Game");
    Game2048 game;
    GameState state = MENU;
    bool needsRedraw = true;
    
    bool running = true;
    while (running) {
        if (!window.ProcessMessages()) {
            running = false;
            break;
        }
        
        // Handle input based on state
        if (window.IsKeyPressed(VK_RETURN)) {
            if (state == MENU) {
                state = PLAYING;
                game = Game2048();  // Reset game
                needsRedraw = true;
            } else if (state == GAME_OVER) {
                state = PLAYING;
                game = Game2048();  // Reset game
                needsRedraw = true;
            }
            window.ClearKeyPressed(VK_RETURN);
        }
        
        if (state == PLAYING) {
            bool moved = false;
            if (window.IsKeyPressed(VK_LEFT)) {
                moved = game.MoveLeft();
                window.ClearKeyPressed(VK_LEFT);
            } else if (window.IsKeyPressed(VK_RIGHT)) {
                moved = game.MoveRight();
                window.ClearKeyPressed(VK_RIGHT);
            } else if (window.IsKeyPressed(VK_UP)) {
                moved = game.MoveUp();
                window.ClearKeyPressed(VK_UP);
            } else if (window.IsKeyPressed(VK_DOWN)) {
                moved = game.MoveDown();
                window.ClearKeyPressed(VK_DOWN);
            }
            
            if (moved) {
                game.AddRandomTile();
                needsRedraw = true;
                
                if (!game.HasPossibleMoves()) {
                    state = GAME_OVER;
                }
            }
        }
        
        // Render only when needed
        if (needsRedraw) {
            HDC hdc = window.GetDrawContext();
            
            if (state == MENU) {
                // Draw menu
                window.DrawRoundedRect(hdc, -5, -5, 405, 405, 2, 0, 0, 0);
                window.DrawText(hdc, 0, 80, 400, 140, "2048", 48, 255, 255, 255);
                window.DrawText(hdc, 0, 200, 400, 240, "Press [ENTER] to Start", 24, 200, 200, 200);
            } else {
                // Draw game
                window.DrawRoundedRect(hdc, 0, 0, 400, 400, 2, 70, 65, 50);
                
                int spacing = 10;
                int cellSize = (400 - (spacing * 5)) / 4;
                
                for (int row = 0; row < 4; row++) {
                    for (int col = 0; col < 4; col++) {
                        int x = spacing + col * (cellSize + spacing);
                        int y = spacing + row * (cellSize + spacing);
                        
                        if (game.grid[row][col] != 0) {
                            int r, g, b;
                            game.Get2048Color(game.grid[row][col], r, g, b);
                            window.DrawRoundedRect(hdc, x, y, x + cellSize, y + cellSize, 2, r, g, b);
                            
                            char text[16];
                            sprintf(text, "%d", game.grid[row][col]);
                            
                            int textR, textG, textB;
                            game.GetTextColor(game.grid[row][col], textR, textG, textB);
                            window.DrawText(hdc, x, y, x + cellSize, y + cellSize, text, 36, textR, textG, textB);
                        } else {
                            window.DrawRoundedRect(hdc, x, y, x + cellSize, y + cellSize, 2, 140, 130, 100);
                        }
                    }
                }
                
                if (state == GAME_OVER) {
                    // Draw game over overlay
                    window.DrawRoundedRect(hdc, 50, 150, 350, 250, 5, 0, 0, 0);
                    window.DrawText(hdc, 50, 160, 350, 200, "You Lost!", 32, 255, 100, 100);
                    window.DrawText(hdc, 50, 210, 350, 240, "Press [ENTER] to try again!", 18, 200, 200, 200);
                }
            }
            
            window.ReleaseDrawContext(hdc);
            needsRedraw = false;
        }
        
        Sleep(10);
    }
    
    std::cout << "Game Closed\n";
    return 0;
}
