#pragma once
#include "IGameUI.h"
#include <string>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

class GameOpenGL : public IGameUI {
public:
    GameOpenGL(int argc, char** argv);
    ~GameOpenGL() override = default;

    PlayerInfo promptPlayerInfo(int playerNumber, char takenSymbol, bool askForType = true) override;
    GameMode askPostGame() override;
    GameMode askGameMode() override;
    BotDifficulty askBotDifficulty() override;
    void printBoard(char board[Board::MAX_HEIGHT][Board::MAX_WIDTH]) override;
    void printWinMessage(Player& player) override;
    void printWelcome() override;
    void printPlayerAdded(const Player& player) override;
    void printDrawMessage() override;
    short getPlayerMove(const std::string& name, char symbol, const char board[3][3]) override;
    void printInvalidMoveMessage() override;
    void printInvalidInputMessage() override;
    void displayGameOver() override;
    void printMessage(const std::string& msg) override;

private:
    // window state
    static int windowWidth;
    static int windowHeight;

    // board state
    static char currentBoard[Board::MAX_HEIGHT][Board::MAX_WIDTH];
    static std::string statusMessage;
    static bool waitingForClick;
    static int clickedCell; // -1 = no click yet, 1-9 = cell clicked

    // GLUT callbacks — must be static for GLUT
    static void display();
    static void mouse(int button, int state, int x, int y);
    static void keyboard(unsigned char key, int x, int y);

    // drawing helpers
    static void drawGrid();
    static void drawSymbols();
    static void drawText(const std::string& text, float x, float y, float r, float g, float b);
    static void drawX(float cx, float cy, float size);
    static void drawO(float cx, float cy, float radius);

    // coordinate helpers
    static int pixelToCell(int x, int y);
};
