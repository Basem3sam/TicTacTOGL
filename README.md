# TicTacTOGL

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![C++14](https://img.shields.io/badge/C%2B%2B-14-blue)](https://en.cppreference.com/w/cpp/compiler_support)
[![OpenGL](https://img.shields.io/badge/Graphics-OpenGL-green)](https://www.opengl.org/)
[![Course](https://img.shields.io/badge/Course-Computer%20Graphics-orange)]()

---

A classic Tic Tac Toe game built in **C++14** with an **OpenGL + freeGLUT** graphical interface and a fully playable **terminal mode**. Developed as a **practical exam project** for the Computer Graphics course. Designed with clean object-oriented architecture and a real AI opponent powered by the **Minimax algorithm**.

---

## Team Members

| Name | Role |
|---|---|
| Basem Esam | |
| Ahmed Abdallah | |
| Ahmed Essam | |
| Ahmed Faraj | |
| Khaled Mohamed | |
| Mohamed Haggag | |
| Ahmed | |

---

## Features

- **Two game modes** — Single Player (vs AI) and Multiplayer (vs friend)
- **Three AI difficulty levels**
  - Easy — random moves
  - Medium — rule-based strategy (win, block, center, corner)
  - Hard — unbeatable Minimax algorithm
- **Two UI modes** — OpenGL graphical window and terminal fallback
- **Post-game menu** — replay, change mode, change difficulty, or quit
- **Clean OOP architecture** — game logic fully separated from UI via interface

---

## Requirements

- **Code::Blocks 17.12** with the bundled MinGW compiler
- **freeGLUT** (configured as described below)

---

## freeGLUT Setup

This project was built and tested with `freeglut-MSVC-3.0.0-2` configured inside Code::Blocks MinGW. Follow these steps **exactly** — every team member must do this before building.

### 1. Download freeGLUT

Download `freeglut-MSVC-3.0.0-2.zip` and extract it.

### 2. Copy MinGW libraries

Copy everything inside `freeglut/lib/mingw/` into:
```
C:\Program Files (x86)\CodeBlocks\MinGW\lib\
```

Copy headers from `freeglut/include/` into:
```
C:\Program Files (x86)\CodeBlocks\MinGW\include\
```

### 3. Copy bin contents to two locations

Copy everything inside `freeglut/bin/` into **both** of these:

Into Code::Blocks MinGW:
```
C:\Program Files (x86)\CodeBlocks\MinGW\bin\
```

Into Windows system (so the DLL is found at runtime):
```
C:\Windows\SysWOW64\
```

### 4. Patch the Code::Blocks GLUT template

Open this file:
```
C:\Program Files (x86)\CodeBlocks\share\CodeBlocks\templates\glut.cbp
```
Find every occurrence of `glut32` and replace with `freeglut`.

Then open:
```
C:\Program Files (x86)\CodeBlocks\share\CodeBlocks\templates\wizard\glut\wizard.script
```
Find every occurrence of `glut32` and replace with `freeglut`.

---

## Building the Project

**Every team member:**

```
1. Complete the freeGLUT setup above
2. git clone https://github.com/Basem3sam/TicTacTOGL.git
3. Open TicTacTOGL.cbp in Code::Blocks
4. Press F9 to build and run
```

> If you get linker errors, double check step 2 and 3 of freeGLUT setup — the lib files and DLL must be in the exact paths listed.

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
│   └── UISwitcher.h/.cpp     # Selects terminal or OpenGL at startup
└── utils/
    ├── Foundation.h/.cpp     # Board struct
    ├── GameMode.h            # GameMode enum
    ├── BotDifficulty.h       # BotDifficulty enum
    ├── PlayerInfo.h          # PlayerInfo data struct
    ├── TerminalStyle.h       # ANSI color macros
    ├── TextUtils.h           # slowPrint, sleepMilliSec utilities
    └── Validation.h/.cpp     # Input and move validation
```

---

## Architecture

The project follows the **Dependency Injection** and **Interface Segregation** principles from SOLID:

```
main()
  └── UISwitcher → creates GameUI (terminal) or GameOpenGL (graphical)
  └── Game receives UI via IGameUI* — never knows which one it has
        ├── setup()      → collects player info through UI
        ├── singlePlay() → Human vs AI game loop
        └── multiPlay()  → Human vs Human game loop
```

This means the entire game logic in `Game`, `Bot`, `Human`, and `Player` is **completely independent** of whether you're running in terminal or OpenGL mode.

---

## How the AI Works

The Hard difficulty uses the **Minimax algorithm** — it explores every possible future game state recursively and always picks the optimal move. On a 3x3 board it is mathematically unbeatable.

Scores are weighted by depth so the AI prefers winning faster:
- Win → `10 - depth`
- Loss → `depth - 10`
- Draw → `0`

---

## Controls

| Action | Control |
|---|---|
| Place symbol | Left mouse click on a cell |
| Exit | ESC key |
| Menu navigation | Keyboard input in terminal |

---

## Team Workflow (Git)

**Pull latest before working:**
```bash
git pull
```

**Push your changes:**
```bash
git add .
git commit -m "feat: your change description"
git push
```

**Never commit `bin/` or `obj/` folders** — they are in `.gitignore` and contain compiled output that differs per machine.

---

## License

This project is licensed under the **MIT License** — see the [LICENSE](LICENSE) file for details.

---

> Practical Exam Project — Computer Graphics Course
> Team of 7 — Built with C++14, OpenGL, and freeGLUT
