# TicTacTOGL

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![C++14](https://img.shields.io/badge/C%2B%2B-14-blue)](https://en.cppreference.com/w/cpp/compiler_support)
[![OpenGL](https://img.shields.io/badge/Graphics-OpenGL-green)](https://www.opengl.org/)
[![Author](https://img.shields.io/badge/Author-Basem%20Esam-purple)](https://github.com/Basem3sam)

---

A classic Tic Tac Toe game built in **C++14** with an **OpenGL + freeGLUT** graphical interface and a fully playable **terminal mode**. Designed with clean object-oriented architecture and a real AI opponent powered by the **Minimax algorithm**.

---

## Features

- **Two game modes** — Single Player (vs AI) and Multiplayer (vs friend)
- **Three AI difficulty levels**
  - Easy — random moves
  - Medium — rule-based strategy (win, block, center, corner)
  - Hard — unbeatable Minimax algorithm
- **Two UI modes** — OpenGL graphical window and terminal fallback
- **Post-game menu** — replay, change mode, change difficulty, or quit
- **Clean architecture** — game logic fully separated from UI via interface

---

## Requirements

- **Code::Blocks 17.12** with the bundled MinGW compiler
- **freeGLUT** (configured as described below)

---

## freeGLUT Setup

This project was built and tested with `freeglut-MSVC-3.0.0-2` configured inside Code::Blocks MinGW. Follow these steps exactly to replicate the setup:

**1. Download freeGLUT**

Download `freeglut-MSVC-3.0.0-2.zip` and extract it.

**2. Copy the MinGW libraries**

Copy everything inside the extracted `freeglut/lib/mingw/` folder into:
```
C:\Program Files (x86)\CodeBlocks\MinGW\lib\
```

Copy the headers from `freeglut/include/` into:
```
C:\Program Files (x86)\CodeBlocks\MinGW\include\
```

**3. Copy the DLL to system**

Copy `freeglut.dll` (from `freeglut/bin/`) into:
```
C:\Windows\SysWOW64\
```

**4. Patch the Code::Blocks GLUT template**

Open:
```
C:\Program Files (x86)\CodeBlocks\share\CodeBlocks\templates\glut.cbp
```
Find `glut32` and replace it with `freeglut`.

Then open:
```
C:\Program Files (x86)\CodeBlocks\share\CodeBlocks\templates\wizard\glut\wizard.script
```
Find `glut32` and replace it with `freeglut`.

---

## Building the Project

1. Clone the repository:
```
git clone https://github.com/Basem3sam/TicTacTOGL.git
```

2. Open `TicTacTOGL.cbp` in Code::Blocks

3. Press **F9** to build and run

> Make sure freeGLUT is set up correctly before building — see setup above.

---

## Project Structure

```
TicTacTOGL/
├── main.cpp                  # Entry point
├── core/
│   ├── Game.h                # Game class declaration
│   └── Game.cpp              # Game logic — setup, play loop, post-game
├── entities/
│   ├── Player.h/.cpp         # Base player class
│   ├── Human.h/.cpp          # Human player
│   └── Bot.h/.cpp            # AI player with Minimax
├── ui/
│   ├── IGameUI.h             # UI interface — 14 pure virtual methods
│   ├── GameUI.h/.cpp         # Terminal UI implementation
│   ├── GameOpenGL.h/.cpp     # OpenGL UI implementation
│   ├── UISwitcher.h/.cpp     # Selects terminal or OpenGL at startup
└── utils/
    ├── Foundation.h/.cpp     # Board struct
    ├── GameMode.h            # GameMode enum
    ├── BotDifficulty.h       # BotDifficulty enum
    ├── PlayerInfo.h          # PlayerInfo data struct
    ├── TerminalStyle.h       # ANSI color macros
    ├── TextUtils.h           # slowPrint, sleepMilliSec utilities
    ├── Validation.h/.cpp     # Input and move validation
    └── TicTacTOGL.cbp        # Code::Blocks project file
```

---

## How the AI Works

The Hard difficulty uses the **Minimax algorithm** — it explores every possible future game state recursively and picks the move that guarantees the best outcome regardless of what the opponent does. On a 3x3 board it is mathematically unbeatable.

Scores are weighted by depth so the AI prefers winning faster and losing later:
- Win → `10 - depth`
- Loss → `depth - 10`
- Draw → `0`

---

## Controls

| Action | Input |
|---|---|
| Place symbol | Left mouse click on a cell |
| Exit | ESC key |
| Menu selection | Keyboard input in terminal |

---

## License

This project is licensed under the **MIT License** — see the [LICENSE](LICENSE) file for details.

---

> Built with love by **Basem Esam**
