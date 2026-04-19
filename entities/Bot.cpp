#include "Bot.h"
#include <vector>
#include <algorithm>
#include "../utils/Validation.h"
#include <cstdlib>

// Helper to get available moves
std::vector<short> getAvailableMoves(const char board[Board::MAX_HEIGHT][Board::MAX_WIDTH]) {
    std::vector<short> moves;
    for (short i = 0; i < Board::MAX_HEIGHT; ++i) {
        for (short j = 0; j < Board::MAX_WIDTH; ++j) {
            if (!(board[i][j] == 'X' || board[i][j] == 'O')) {
                moves.push_back(i * Board::MAX_WIDTH + j + 1);
            }
        }
    }
    return moves;
}

// place a move on a copy of the board
static void applyMove(char board[3][3], short move, char symbol) {
    short row = (move - 1) / Board::MAX_WIDTH;
    short col = (move - 1) % Board::MAX_WIDTH;
    board[row][col] = symbol;
}

// ─── Medium: rule-based ─────────────────────────────────────────────────────

// returns winning move for `symbol` if one exists, else -1
static short findWinningMove(const char board[3][3], char symbol) {
    auto moves = getAvailableMoves(board);
    for (short move : moves) {
        char copy[3][3];
        std::copy(&board[0][0], &board[0][0] + 9, &copy[0][0]);
        applyMove(copy, move, symbol);
        if (Valid::isWin(copy, symbol))
            return move;
    }
    return -1;
}

static short mediumMove(const char board[3][3], char mySymbol) {
    char opponentSymbol = (mySymbol == 'X') ? 'O' : 'X';

    // 1. win if possible
    short win = findWinningMove(board, mySymbol);
    if (win != -1) return win;

    // 2. block opponent's win
    short block = findWinningMove(board, opponentSymbol);
    if (block != -1) return block;

    // 3. take center
    if (board[1][1] != 'X' && board[1][1] != 'O') return 5;

    // 4. take a corner
    for (short corner : {1, 3, 7, 9}) {
        short r = (corner-1)/3, c = (corner-1)%3;
        if (board[r][c] != 'X' && board[r][c] != 'O') return corner;
    }

    // 5. take anything
    return getAvailableMoves(board)[0];
}

// ─── Hard: Minimax ──────────────────────────────────────────────────────────

static int minimax(char board[3][3], char mySymbol, char current, int depth) {
    char opponent = (mySymbol == 'X') ? 'O' : 'X';

    if (Valid::isWin(board, mySymbol))   return 10 - depth;
    if (Valid::isWin(board, opponent))   return depth - 10;

    auto moves = getAvailableMoves(board);
    if (moves.empty()) return 0; // draw

    if (current == mySymbol) {
        // maximizing
        int best = -100;
        for (short move : moves) {
            char copy[3][3];
            std::copy(&board[0][0], &board[0][0] + 9, &copy[0][0]);
            applyMove(copy, move, current);
            best = std::max(best, minimax(copy, mySymbol, opponent, depth + 1));
        }
        return best;
    } else {
        // minimizing
        int best = 100;
        for (short move : moves) {
            char copy[3][3];
            std::copy(&board[0][0], &board[0][0] + 9, &copy[0][0]);
            applyMove(copy, move, current);
            best = std::min(best, minimax(copy, mySymbol, mySymbol, depth + 1));
        }
        return best;
    }
}

static short hardMove(const char board[3][3], char mySymbol) {
    char opponent = (mySymbol == 'X') ? 'O' : 'X';
    auto moves = getAvailableMoves(board);

    short bestMove = moves[0];
    int bestScore = -100;

    for (short move : moves) {
        char copy[3][3];
        std::copy(&board[0][0], &board[0][0] + 9, &copy[0][0]);
        applyMove(copy, move, mySymbol);
        int score = minimax(copy, mySymbol, opponent, 0);
        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }
    return bestMove;
}

short Bot::decideMove(const char platform[Board::MAX_HEIGHT][Board::MAX_WIDTH]) {
    std::vector<short> moves = getAvailableMoves(platform);
    if (moves.empty()) return -1;

    char mySymbol = getSymbol();

    switch (difficulty) {
        case BotDifficulty::Easy:
            return moves[rand() % moves.size()];

        case BotDifficulty::Medium:
            return mediumMove(platform, mySymbol);

        case BotDifficulty::Hard:
            return hardMove(platform, mySymbol);
    }
    return moves[0]; // fallback
}
