# 2048 Game (Win32, C++)

Lightweight 2048 clone written in C++ with raw Win32 and GDI for rendering. A small helper `Window` class wraps the window creation, drawing, and keyboard handling so the game logic stays compact and easy to tweak.

## Features
- 4×4 grid with the standard 2048 merging rules and random 2/4 tile spawns
- Keyboard controls (arrow keys) plus Enter to start or restart
- Simple menu and game-over overlay with the classic 2048 color palette
- Single executable build; no external assets or frameworks required

## Requirements
- Windows (tested with MSYS2/MinGW-w64 toolchain)
- `g++` with access to Win32 libraries (`gdi32`, `user32`, `kernel32`)
- Optional: the included `2048_game.exe` if you only want to play

## Build and Run
### Using the provided script (MSYS2/MinGW-w64)
1) Ensure your compiler path matches the one in `build_and_run.bat` (default: `C:\msys64\ucrt64\bin\g++.exe`).  
2) From a Command Prompt in the project root, run:
```bat
build_and_run.bat
```
The script compiles `main.cpp` and `Window.cpp` statically against the Win32 libs and launches the game on success.

### Manual compile (any MinGW-w64 `g++` on PATH)
```bash
g++ -O2 main.cpp Window.cpp -o 2048_game.exe -lgdi32 -luser32 -lkernel32 -static-libgcc -static-libstdc++
```
Then run `2048_game.exe`.

## Controls
- `Enter` — start from the menu or restart after losing
- Arrow keys — move tiles left/right/up/down
- Window close — exit

## Project Layout
- `main.cpp` — game loop, state machine, tile logic, rendering calls
- `Window.cpp` / `Window.h` — minimal Win32 wrapper (window creation, drawing primitives, keyboard state)
- `build_and_run.bat` — convenience build script for MinGW-w64
- `2048_game.exe` — prebuilt binary (optional)

## Notes
- Rendering uses basic GDI rectangles/text; tweak colors or sizes inside `main.cpp` to adjust the look.
- If you use a different toolchain location, update the compiler path in `build_and_run.bat` or invoke `g++` manually as shown above.
