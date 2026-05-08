#include "Game.h"
#include "Pieces.h"
#include <iostream>
#include <sstream>
#include <cctype>
using namespace std;

Game::Game() : currentTurn('W'), gameOver(false) {
    board.initializeBoard();
}

void Game::start() {
    cout << "========================================\n";
    cout << "      WELCOME TO C++ CHESS GAME\n";
    cout << "========================================\n";
    cout << "  Format: e2 e4  (from-square to-square)\n";
    cout << "  Columns: a-h   Rows: 1-8\n";
    cout << "  Type 'quit' to exit\n";
    cout << "========================================\n";
    board.display();

    while (!gameOver) {
        printTurnInfo();

        int fr, fc, tr, tc;
        if (!getMove(fr, fc, tr, tc)) continue;
        if (!applyMove(fr, fc, tr, tc)) continue;

        switchTurn();

        if (board.isCheckmate(currentTurn)) {
            board.display();
            switchTurn();
            printWinner();
            gameOver = true;
        }
        else if (board.isStalemate(currentTurn)) {
            board.display();
            cout << "  ** STALEMATE! It's a draw. **\n";
            gameOver = true;
        }
        else {
            board.display();
            if (board.isInCheck(currentTurn))
                cout << "  ** CHECK! **\n";
        }
    }
    cout << "\nGame over. Thanks for playing!\n";
}

void Game::printTurnInfo() const {
    string player = (currentTurn == 'W') ? "White" : "Black";
    cout << "  [" << player << "'s turn] Enter move: ";
}

bool Game::parsePosition(const string& s, int& row, int& col) const {
    if (s.size() != 2) return false;
    char colChar = tolower(s[0]);
    char rowChar = s[1];
    if (colChar < 'a' || colChar > 'h') return false;
    if (rowChar < '1' || rowChar > '8') return false;
    col = colChar - 'a';
    row = 8 - (rowChar - '0');
    return true;
}

bool Game::getMove(int& fromRow, int& fromCol, int& toRow, int& toCol) {
    string line;
    getline(cin, line);

    if (line == "quit" || line == "exit") {
        gameOver = true;
        return false;
    }

    istringstream ss(line);
    string from, to;
    ss >> from >> to;

    if (from.empty() || to.empty()) {
        cout << "  Bad input. Example: e2 e4\n";
        return false;
    }
    if (!parsePosition(from, fromRow, fromCol) ||
        !parsePosition(to, toRow, toCol)) {
        cout << "  Invalid square. Use a-h and 1-8 (e.g., e2 e4)\n";
        return false;
    }
    return true;
}

bool Game::applyMove(int fr, int fc, int tr, int tc) {
    Piece* piece = board.getPiece(fr, fc);

    if (piece == nullptr) {
        cout << "  No piece at that square.\n";
        return false;
    }
    if (piece->getColor() != currentTurn) {
        cout << "  That's not your piece!\n";
        return false;
    }

    // Build local grid snapshot
    Piece* tempGrid[8][8];
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++)
            tempGrid[r][c] = board.getPiece(r, c);

    if (!piece->isValidMove(tr, tc, tempGrid)) {
        cout << "  Invalid move for that piece!\n";
        return false;
    }

    // Simulate to ensure King not left in check
    Piece* simGrid[8][8];
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++)
            simGrid[r][c] = tempGrid[r][c];

    simGrid[tr][tc] = simGrid[fr][fc];
    simGrid[fr][fc] = nullptr;
    if (simGrid[tr][tc])
        simGrid[tr][tc]->setPosition(tr, tc);

    int kr = -1, kc = -1;
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++)
            if (simGrid[r][c] && simGrid[r][c]->getColor() == currentTurn
                && simGrid[r][c]->getSymbol().substr(1) == "K")
            {
                kr = r; kc = c;
            }

    bool leavesInCheck = false;
    char opp = (currentTurn == 'W') ? 'B' : 'W';
    if (kr != -1)
        for (int r = 0; r < 8; r++)
            for (int c = 0; c < 8; c++)
                if (simGrid[r][c] && simGrid[r][c]->getColor() == opp)
                    if (simGrid[r][c]->isValidMove(kr, kc, simGrid))
                        leavesInCheck = true;

    // Undo position change on sim piece
    if (simGrid[tr][tc])
        simGrid[tr][tc]->setPosition(fr, fc);

    if (leavesInCheck) {
        cout << "  Illegal! That move would leave your King in check.\n";
        return false;
    }

    // Actual move
    Piece* captured = board.movePiece(fr, fc, tr, tc);
    if (captured) {
        cout << "  Captured: " << captured->getSymbol() << "\n";
        delete captured;
    }
    return true;
}

void Game::switchTurn() {
    currentTurn = (currentTurn == 'W') ? 'B' : 'W';
}

void Game::printWinner() const {
    string winner = (currentTurn == 'W') ? "White" : "Black";
    cout << "\n  *** CHECKMATE! " << winner << " WINS! ***\n";
}