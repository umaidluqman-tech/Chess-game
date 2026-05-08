#pragma once
#ifndef BOARD_H
#define BOARD_H

#include "Piece.h"
#include <string>
using namespace std;

// ============================================================
//  BOARD CLASS  –  Demonstrates: Composition
//  The Board "contains" Piece objects (composition relationship)
// ============================================================
class Board {
private:
    Piece* grid[8][8];   // 8x8 grid of piece pointers (nullptr = empty)

public:
    Board();
    ~Board();

    void  initializeBoard();           // Place all pieces at start
    void  display() const;             // Print board to console

    Piece* getPiece(int row, int col) const;
    void   setPiece(int row, int col, Piece* piece);

    // Move a piece; returns captured piece (or nullptr)
    Piece* movePiece(int fromRow, int fromCol, int toRow, int toCol);

    // Check if a given color's King is under attack
    bool isInCheck(char color) const;

    // Check if the given color has no legal moves
    bool isCheckmate(char color) const;
    bool isStalemate(char color) const;

private:
    // Helper: can 'color' make ANY legal move?
    bool hasLegalMove(char color) const;
};

#endif
