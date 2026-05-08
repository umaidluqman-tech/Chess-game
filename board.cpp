#include "board.h"
#include "pieces.h"
#include <iostream>
#include <iomanip>
using namespace std;

// ----------------------------------------------------------
Board::Board() {
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++)
            grid[r][c] = nullptr;
}

// ----------------------------------------------------------
Board::~Board() {
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++)
            delete grid[r][c];
}

// ----------------------------------------------------------
void Board::initializeBoard() {
    // Clear first
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++) {
            delete grid[r][c];
            grid[r][c] = nullptr;
        }

    // --- Black pieces (top, rows 0-1) ---
    grid[0][0] = new Rook('B', 0, 0);
    grid[0][1] = new Knight('B', 0, 1);
    grid[0][2] = new Bishop('B', 0, 2);
    grid[0][3] = new Queen('B', 0, 3);
    grid[0][4] = new King('B', 0, 4);
    grid[0][5] = new Bishop('B', 0, 5);
    grid[0][6] = new Knight('B', 0, 6);
    grid[0][7] = new Rook('B', 0, 7);
    for (int c = 0; c < 8; c++)
        grid[1][c] = new Pawn('B', 1, c);

    // --- White pieces (bottom, rows 6-7) ---
    for (int c = 0; c < 8; c++)
        grid[6][c] = new Pawn('W', 6, c);
    grid[7][0] = new Rook('W', 7, 0);
    grid[7][1] = new Knight('W', 7, 1);
    grid[7][2] = new Bishop('W', 7, 2);
    grid[7][3] = new Queen('W', 7, 3);
    grid[7][4] = new King('W', 7, 4);
    grid[7][5] = new Bishop('W', 7, 5);
    grid[7][6] = new Knight('W', 7, 6);
    grid[7][7] = new Rook('W', 7, 7);
}

// ----------------------------------------------------------
void Board::display() const {
    const string BORDER = "  +----+----+----+----+----+----+----+----+";
    const string FILES = "     a    b    c    d    e    f    g    h";

    cout << "\n" << FILES << "\n";
    cout << BORDER << "\n";

    for (int r = 0; r < 8; r++) {
        cout << (8 - r) << " |";
        for (int c = 0; c < 8; c++) {
            if (grid[r][c] == nullptr)
                cout << "    |";
            else {
                string sym = grid[r][c]->getSymbol();
                if (sym.length() == 1)
                    cout << " " << sym << "  |";
                else
                    cout << " " << sym << " |";
            }
        }
        cout << " " << (8 - r) << "\n";
        cout << BORDER << "\n";
    }

    cout << FILES << "\n\n";
}
// ----------------------------------------------------------
Piece* Board::getPiece(int row, int col) const {
    if (row < 0 || row > 7 || col < 0 || col > 7) return nullptr;
    return grid[row][col];
}

void Board::setPiece(int row, int col, Piece* piece) {
    grid[row][col] = piece;
}

// ----------------------------------------------------------
Piece* Board::movePiece(int fromRow, int fromCol, int toRow, int toCol) {
    Piece* captured = grid[toRow][toCol];   // May be nullptr

    // Move piece
    grid[toRow][toCol] = grid[fromRow][fromCol];
    grid[fromRow][fromCol] = nullptr;

    // Update the piece's internal position
    if (grid[toRow][toCol])
        grid[toRow][toCol]->setPosition(toRow, toCol);

    return captured;
}

// ----------------------------------------------------------
// isInCheck: is the 'color' King currently attacked?
bool Board::isInCheck(char color) const {
    // Find the King
    int kingRow = -1, kingCol = -1;
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++)
            if (grid[r][c] != nullptr
                && grid[r][c]->getColor() == color
                && grid[r][c]->getSymbol().substr(1) == "K")
            {
                kingRow = r;  kingCol = c;
            }

    if (kingRow == -1) return false;   // King not found (shouldn't happen)

    // Check every opponent piece
    char opponent = (color == 'W') ? 'B' : 'W';
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++)
            if (grid[r][c] != nullptr && grid[r][c]->getColor() == opponent)
                if (grid[r][c]->isValidMove(kingRow, kingCol, grid))
                    return true;

    return false;
}

// ----------------------------------------------------------
// hasLegalMove: does 'color' have at least one move that doesn't leave King in check?
bool Board::hasLegalMove(char color) const {
    for (int fr = 0; fr < 8; fr++)
        for (int fc = 0; fc < 8; fc++) {
            if (grid[fr][fc] == nullptr || grid[fr][fc]->getColor() != color)
                continue;

            // Try every destination
            for (int tr = 0; tr < 8; tr++)
                for (int tc = 0; tc < 8; tc++) {
                    if (!grid[fr][fc]->isValidMove(tr, tc, grid)) continue;

                    // Simulate the move
                    Piece* tempBoard[8][8];
                    for (int r = 0; r < 8; r++)
                        for (int c = 0; c < 8; c++)
                            tempBoard[r][c] = grid[r][c];

                    Piece* captured = tempBoard[tr][tc];
                    tempBoard[tr][tc] = tempBoard[fr][fc];
                    tempBoard[fr][fc] = nullptr;
                    if (tempBoard[tr][tc])
                        tempBoard[tr][tc]->setPosition(tr, tc);

                    // Find King after move
                    int kr = -1, kc = -1;
                    for (int r = 0; r < 8; r++)
                        for (int c = 0; c < 8; c++)
                            if (tempBoard[r][c] && tempBoard[r][c]->getColor() == color
                                && tempBoard[r][c]->getSymbol().substr(1) == "K")
                            {
                                kr = r; kc = c;
                            }

                    bool stillInCheck = false;
                    if (kr != -1) {
                        char opp = (color == 'W') ? 'B' : 'W';
                        for (int r = 0; r < 8; r++)
                            for (int c = 0; c < 8; c++)
                                if (tempBoard[r][c] && tempBoard[r][c]->getColor() == opp)
                                    if (tempBoard[r][c]->isValidMove(kr, kc, tempBoard))
                                        stillInCheck = true;
                    }

                    // Undo position change
                    if (tempBoard[tr][tc])
                        tempBoard[tr][tc]->setPosition(fr, fc);

                    if (!stillInCheck) return true;   // Found a legal move
                }
        }
    return false;
}

bool Board::isCheckmate(char color) const {
    return isInCheck(color) && !hasLegalMove(color);
}

bool Board::isStalemate(char color) const {
    return !isInCheck(color) && !hasLegalMove(color);
}