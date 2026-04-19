#include "GameOpenGL.h"
#include "../utils/Validation.h"
#include "../utils/TextUtils.h"
#include <iostream>
#include <cmath>
#include <cstring>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

// ── static member definitions ───────────────────────────────────────────────
int         GameOpenGL::windowWidth  = 600;
int         GameOpenGL::windowHeight = 700;
char        GameOpenGL::currentBoard[Board::MAX_HEIGHT][Board::MAX_WIDTH] = {};
std::string GameOpenGL::statusMessage = "Welcome!";
bool        GameOpenGL::waitingForClick = false;
int         GameOpenGL::clickedCell     = -1;

// ── constructor ─────────────────────────────────────────────────────────────
GameOpenGL::GameOpenGL(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("XO++ | Tic Tac Toe");

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // dark background
}

// ── GLUT callbacks ───────────────────────────────────────────────────────────
void GameOpenGL::display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // set up 2D orthographic projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    drawGrid();
    drawSymbols();

    // status message at bottom
    drawText(statusMessage, 10.0f, 20.0f, 1.0f, 1.0f, 1.0f);

    glutSwapBuffers();
}

void GameOpenGL::mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && waitingForClick) {
        // GLUT y is from top, OpenGL y is from bottom — flip it
        int flippedY = windowHeight - y;
        int cell = pixelToCell(x, flippedY);
        if (cell != -1) {
            clickedCell = cell;
            waitingForClick = false;
        }
    }
}

void GameOpenGL::keyboard(unsigned char key, int x, int y) {
    if (key == 27) { // ESC
        exit(0);
    }
}

// ── drawing helpers ──────────────────────────────────────────────────────────
void GameOpenGL::drawGrid() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(3.0f);

    glBegin(GL_LINES);
        // horizontal lines — board occupies y: 100 to 700, divided into 3 rows
        glVertex2f(0,   333.0f + 100); glVertex2f(600, 333.0f + 100);
        glVertex2f(0,   166.0f + 100); glVertex2f(600, 166.0f + 100);

        // vertical lines
        glVertex2f(200, 100); glVertex2f(200, 700);
        glVertex2f(400, 100); glVertex2f(400, 700);
    glEnd();
}

void GameOpenGL::drawSymbols() {
    // cell centers — board from y=100 to y=700, x=0 to x=600
    // row 0 = top = y 566..700, row 1 = middle, row 2 = bottom = y 100..266
    for (int row = 0; row < Board::MAX_HEIGHT; ++row) {
        for (int col = 0; col < Board::MAX_WIDTH; ++col) {
            char c = currentBoard[row][col];
            if (c != 'X' && c != 'O') continue;

            // center of cell in OpenGL coords
            float cx = col * 200.0f + 100.0f;
            // row 0 = top, so invert: row 0 maps to highest y
            float cy = (2 - row) * 200.0f + 100.0f + 100.0f;

            if (c == 'X') {
                glColor3f(0.9f, 0.2f, 0.2f); // red
                drawX(cx, cy, 60.0f);
            } else {
                glColor3f(0.2f, 0.5f, 0.9f); // blue
                drawO(cx, cy, 60.0f);
            }
        }
    }
}

void GameOpenGL::drawX(float cx, float cy, float size) {
    glLineWidth(8.0f);
    glBegin(GL_LINES);
        glVertex2f(cx - size, cy - size);
        glVertex2f(cx + size, cy + size);
        glVertex2f(cx + size, cy - size);
        glVertex2f(cx - size, cy + size);
    glEnd();
}

void GameOpenGL::drawO(float cx, float cy, float radius) {
    glLineWidth(8.0f);
    glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 64; ++i) {
            float angle = 2.0f * 3.14159f * i / 64.0f;
            glVertex2f(cx + radius * cos(angle),
                       cy + radius * sin(angle));
        }
    glEnd();
}

void GameOpenGL::drawText(const std::string& text, float x, float y,
                           float r, float g, float b) {
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

// ── coordinate helper ────────────────────────────────────────────────────────
int GameOpenGL::pixelToCell(int x, int y) {
    // board occupies x: 0-600, y: 100-700
    if (x < 0 || x > 600 || y < 100 || y > 700) return -1;

    int col = x / 200;         // 0, 1, or 2
    int row = 2 - ((y - 100) / 200); // flip: top row = row 0

    if (col < 0 || col > 2 || row < 0 || row > 2) return -1;

    return row * 3 + col + 1; // 1-indexed
}

// ── IGameUI implementation ───────────────────────────────────────────────────
void GameOpenGL::printWelcome() {
    statusMessage = "Welcome to XO++! Click a cell to play.";
    glutPostRedisplay();
    TextUtils::sleepMilliSec(1000);
}

GameMode GameOpenGL::askGameMode() {
    // For now ask in terminal, display in OpenGL
    std::cout << "\n[1] Single Player  [2] Multiplayer  [3] Exit\nChoice: ";
    std::string input;
    std::getline(std::cin, input);
    if (input == "1") return GameMode::SinglePlayer;
    if (input == "2") return GameMode::MultiPlayer;
    return GameMode::Return;
}

BotDifficulty GameOpenGL::askBotDifficulty() {
    std::cout << "[1] Easy  [2] Medium  [3] Hard\nChoice: ";
    std::string input;
    std::getline(std::cin, input);
    if (input == "2") return BotDifficulty::Medium;
    if (input == "3") return BotDifficulty::Hard;
    return BotDifficulty::Easy;
}

PlayerInfo GameOpenGL::promptPlayerInfo(int playerNumber, char takenSymbol, bool askForType) {
    PlayerInfo info;
    std::cout << "Player " << playerNumber << " name: ";
    std::getline(std::cin, info.name);
    if (info.name.empty()) info.name = "Player " + std::to_string(playerNumber);

    info.symbol = (takenSymbol == '\0') ? 'X' :
                  (takenSymbol == 'X')  ? 'O' : 'X';

    info.type = PlayerType::Human;
    return info;
}

void GameOpenGL::printBoard(char board[Board::MAX_HEIGHT][Board::MAX_WIDTH]) {
    // copy board into static member so display() can draw it
    for (int i = 0; i < Board::MAX_HEIGHT; ++i)
        for (int j = 0; j < Board::MAX_WIDTH; ++j)
            currentBoard[i][j] = board[i][j];

    glutPostRedisplay();
    glutMainLoopEvent(); // process pending events and redraw
    TextUtils::sleepMilliSec(100);
}

short GameOpenGL::getPlayerMove(const std::string& name, char symbol,
                                 const char board[3][3]) {
    statusMessage = name + "'s turn (" + symbol + ") - click a cell";
    glutPostRedisplay();

    // copy board so display() is up to date
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            currentBoard[i][j] = board[i][j];

    // wait for a valid click
    clickedCell = -1;
    waitingForClick = true;

    while (waitingForClick) {
        glutMainLoopEvent(); // pump the event loop
        TextUtils::sleepMilliSec(16); // ~60fps
    }

    return (short)clickedCell;
}

void GameOpenGL::printWinMessage(Player& player) {
    if (player.getRule() == "Bot") {
        statusMessage = "BOT wins! Better luck next time.";
    } else {
        statusMessage = player.getName() + " WINS!";
    }
    glutPostRedisplay();
    glutMainLoopEvent();
    TextUtils::sleepMilliSec(2000);
}

void GameOpenGL::printDrawMessage() {
    statusMessage = "It's a DRAW!";
    glutPostRedisplay();
    glutMainLoopEvent();
    TextUtils::sleepMilliSec(2000);
}

void GameOpenGL::printPlayerAdded(const Player& player) {
    statusMessage = player.getName() + " joined as " + player.getSymbol();
    glutPostRedisplay();
    glutMainLoopEvent();
    TextUtils::sleepMilliSec(800);
}

void GameOpenGL::printInvalidMoveMessage() {
    statusMessage = "Invalid move! Try again.";
    glutPostRedisplay();
    glutMainLoopEvent();
    TextUtils::sleepMilliSec(800);
}

void GameOpenGL::printInvalidInputMessage() {
    statusMessage = "Invalid input!";
    glutPostRedisplay();
    glutMainLoopEvent();
    TextUtils::sleepMilliSec(800);
}

void GameOpenGL::printMessage(const std::string& msg) {
    statusMessage = msg;
    glutPostRedisplay();
    glutMainLoopEvent();
    TextUtils::sleepMilliSec(1500);
}

GameMode GameOpenGL::askPostGame() {
    std::cout << "\n[1] Replay  [2] Change Mode  [3] Change Difficulty  [4] Quit\nChoice: ";
    std::string input;
    std::getline(std::cin, input);
    if (input == "1") return GameMode::Replay;
    if (input == "2") return GameMode::ChangeMode;
    if (input == "3") return GameMode::ChangeDifficulty;
    return GameMode::Return;
}

void GameOpenGL::displayGameOver() {
    statusMessage = "Game Over! Thanks for playing.";
    glutPostRedisplay();
    glutMainLoopEvent();
    TextUtils::sleepMilliSec(3000);
}
