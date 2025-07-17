#include "Window.h"
#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <ctime>

// Movement functions
void MoveUp(int values[4][4]);
void MoveDown(int values[4][4]);
void MoveLeft(int values[4][4]);
void MoveRight(int values[4][4]);
void AddRandomTile(int values[4][4]);
bool BoardsEqual(int board1[4][4], int board2[4][4]);
bool HasPossibleMoves(int values[4][4]);
void CopyBoard(int source[4][4], int dest[4][4]);

// Global variables for key communication
bool keyPressed = false;
int lastKey = 0;
bool needsRedraw = true;

// Game state
enum GameState {
    MENU,
    PLAYING,
    GAME_OVER
};

GameState currentState = MENU;

int main() {
    std::cout << "Creating Window\n";

    srand(time(NULL));

    // Initialize empty game grid (starts in menu state)
    int values[4][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};

    Window* pWindow = new Window();

    // Initial render needed
    needsRedraw = true;

    bool running = true;
    while (running)
    {
        if (!pWindow->ProcessMessages())
        {
            std::cout << "Closing Window\n";
            running = false;
        }

        // Handle input based on game state
        if (keyPressed) {
            if (currentState == MENU) {
                if (lastKey == VK_RETURN) {  // Enter key starts the game
                    currentState = PLAYING;
                    // Initialize game grid
                    for (int i = 0; i < 4; i++) {
                        for (int j = 0; j < 4; j++) {
                            values[i][j] = 0;
                        }
                    }
                    AddRandomTile(values);
                    AddRandomTile(values);
                    needsRedraw = true;
                    std::cout << "Game Started!\n";
                }
            }
            else if (currentState == PLAYING) {
                // Save board state before move
                int oldBoard[4][4];
                CopyBoard(values, oldBoard);

                bool moved = false;
                switch (lastKey) {
                    case VK_UP:
                        MoveUp(values);
                        moved = !BoardsEqual(values, oldBoard);
                        if (moved) std::cout << "Moved UP\n";
                        break;
                    case VK_DOWN:
                        MoveDown(values);
                        moved = !BoardsEqual(values, oldBoard);
                        if (moved) std::cout << "Moved DOWN\n";
                        break;
                    case VK_LEFT:
                        MoveLeft(values);
                        moved = !BoardsEqual(values, oldBoard);
                        if (moved) std::cout << "Moved LEFT\n";
                        break;
                    case VK_RIGHT:
                        MoveRight(values);
                        moved = !BoardsEqual(values, oldBoard);
                        if (moved) std::cout << "Moved RIGHT\n";
                        break;
                }

                // Only add new tile if board actually changed
                if (moved) {
                    AddRandomTile(values);
                    needsRedraw = true;

                    // Check for game over
                    if (!HasPossibleMoves(values)) {
                        currentState = GAME_OVER;
                        std::cout << "Game Over!\n";
                    }
                }
            }
            else if (currentState == GAME_OVER) {
                if (lastKey == VK_RETURN) {  // Enter key restarts the game
                    currentState = PLAYING;
                    // Reset game grid
                    for (int i = 0; i < 4; i++) {
                        for (int j = 0; j < 4; j++) {
                            values[i][j] = 0;
                        }
                    }
                    AddRandomTile(values);
                    AddRandomTile(values);
                    needsRedraw = true;
                    std::cout << "Game Restarted!\n";
                }
            }
            keyPressed = false;  // Reset flag
        }

        // Only render when something has changed
        if (needsRedraw) {
            if (currentState == MENU) {
                pWindow->RenderMenu();
            } else if (currentState == PLAYING) {
                pWindow->Render(values);
            } else if (currentState == GAME_OVER) {
                pWindow->RenderGameOver(values);
            }
            needsRedraw = false;
        }
        Sleep(10);  // Reduced sleep time for better responsiveness
    }

    delete pWindow;

    return 0;
}

void AddRandomTile(int values[4][4])
{
    // Find empty cells
    int emptyCells[16][2];
    int emptyCount = 0;

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (values[row][col] == 0) {
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

        values[row][col] = value;
        std::cout << "Added " << value << " at (" << row << ", " << col << ")\n";
    }
}

void MoveUp(int values[4][4])
{
    for (int col = 0; col < 4; col++) {
        // Move all tiles up and merge
        int writePos = 0;
        bool merged[4] = {false, false, false, false};

        for (int row = 0; row < 4; row++) {
            if (values[row][col] != 0) {
                if (writePos > 0 && values[writePos-1][col] == values[row][col] && !merged[writePos-1]) {
                    // Merge with the tile above
                    values[writePos-1][col] *= 2;
                    merged[writePos-1] = true;
                    values[row][col] = 0;
                } else {
                    // Move to the next available position
                    if (writePos != row) {
                        values[writePos][col] = values[row][col];
                        values[row][col] = 0;
                    }
                    writePos++;
                }
            }
        }
    }
}

void MoveDown(int values[4][4])
{
    for (int col = 0; col < 4; col++) {
        // Move all tiles down and merge
        int writePos = 3;
        bool merged[4] = {false, false, false, false};

        for (int row = 3; row >= 0; row--) {
            if (values[row][col] != 0) {
                if (writePos < 3 && values[writePos+1][col] == values[row][col] && !merged[writePos+1]) {
                    // Merge with the tile below
                    values[writePos+1][col] *= 2;
                    merged[writePos+1] = true;
                    values[row][col] = 0;
                } else {
                    // Move to the next available position
                    if (writePos != row) {
                        values[writePos][col] = values[row][col];
                        values[row][col] = 0;
                    }
                    writePos--;
                }
            }
        }
    }
}

void MoveLeft(int values[4][4])
{
    for (int row = 0; row < 4; row++) {
        // Move all tiles left and merge
        int writePos = 0;
        bool merged[4] = {false, false, false, false};

        for (int col = 0; col < 4; col++) {
            if (values[row][col] != 0) {
                if (writePos > 0 && values[row][writePos-1] == values[row][col] && !merged[writePos-1]) {
                    // Merge with the tile to the left
                    values[row][writePos-1] *= 2;
                    merged[writePos-1] = true;
                    values[row][col] = 0;
                } else {
                    // Move to the next available position
                    if (writePos != col) {
                        values[row][writePos] = values[row][col];
                        values[row][col] = 0;
                    }
                    writePos++;
                }
            }
        }
    }
}

void MoveRight(int values[4][4])
{
    for (int row = 0; row < 4; row++) {
        // Move all tiles right and merge
        int writePos = 3;
        bool merged[4] = {false, false, false, false};

        for (int col = 3; col >= 0; col--) {
            if (values[row][col] != 0) {
                if (writePos < 3 && values[row][writePos+1] == values[row][col] && !merged[writePos+1]) {
                    // Merge with the tile to the right
                    values[row][writePos+1] *= 2;
                    merged[writePos+1] = true;
                    values[row][col] = 0;
                } else {
                    // Move to the next available position
                    if (writePos != col) {
                        values[row][writePos] = values[row][col];
                        values[row][col] = 0;
                    }
                    writePos--;
                }
            }
        }
    }
}

bool BoardsEqual(int board1[4][4], int board2[4][4])
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (board1[i][j] != board2[i][j]) {
                return false;
            }
        }
    }
    return true;
}

void CopyBoard(int source[4][4], int dest[4][4])
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            dest[i][j] = source[i][j];
        }
    }
}

bool HasPossibleMoves(int values[4][4])
{
    // Check for empty cells
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (values[i][j] == 0) {
                return true;  // Empty cell means moves are possible
            }
        }
    }

    // Check for possible merges (adjacent identical tiles)
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            // Check right neighbor
            if (j < 3 && values[i][j] == values[i][j+1]) {
                return true;
            }
            // Check bottom neighbor
            if (i < 3 && values[i][j] == values[i+1][j]) {
                return true;
            }
        }
    }

    return false;  // No moves possible
}