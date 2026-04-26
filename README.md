# TicTacTOGL (Simplified)

Minimal single-file OpenGL/GLUT Tic Tac Toe for quick course testing.

## Scope

- End-to-end interaction in one window.
- Simple rendering with legacy OpenGL primitives.
- Optional simple AI for `O` player.
- Mouse + keyboard both supported.
- No advanced engine structure required for this version.

## Build In Code::Blocks

1. Open `TicTacTOGL.cbp`.
2. Press `F9` (Build and Run).

If you have freeGLUT configured in Code::Blocks MinGW, it should run directly.

## Controls

- Menu:
  - Mouse: green button starts, red button exits.
  - Keyboard: `1` start, `2` exit.
  - Keyboard: `T` toggles AI mode.
- Game:
  - Mouse: click a cell to place current player symbol.
  - Keyboard: `1..9` place move by cell number.
  - Keyboard: `R` reset round, `M` return to menu, `ESC` exit.
  - Keyboard: `T` toggles AI mode.
- Result:
  - Keyboard: `R` replay, `M` menu, `E` or `ESC` exit.

## AI Notes

- AI is enabled by default.
- When AI is on, `O` plays automatically after `X`.
- Turn AI off with `T` if you want local 2-player style input.

## File Used

- `main.cpp` only.
