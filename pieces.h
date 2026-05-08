#pragma once
#ifndef PIECES_H
#define PIECES_H

#include "Piece.h"
#include <cstdlib>   // abs()

// ============================================================
//  PAWN
// ============================================================
class Pawn : public Piece {
public:
    Pawn(char color, int row, int col) : Piece(color, row, col) {}

    string getSymbol() const override {
        return (color == 'W') ? "WP" : "BP";
    }

    bool isValidMove(int toRow, int toCol, Piece* const board[8][8]) const override {
        int dr = toRow - row;   // row delta
        int dc = toCol - col;   // col delta

        // White moves UP (row decreases), Black moves DOWN (row increases)
        int direction = (color == 'W') ? -1 : 1;

        // One step forward into empty square
        if (dc == 0 && dr == direction && board[toRow][toCol] == nullptr)
            return true;

        // Two steps forward from starting row (White:row6, Black:row1)
        int startRow = (color == 'W') ? 6 : 1;
        if (dc == 0 && dr == 2 * direction && row == startRow
            && board[toRow][toCol] == nullptr
            && board[row + direction][col] == nullptr)
            return true;

        // Diagonal capture
        if (abs(dc) == 1 && dr == direction
            && board[toRow][toCol] != nullptr
            && board[toRow][toCol]->getColor() != color)
            return true;

        return false;
    }
};

// ============================================================
//  ROOK
// ============================================================
class Rook : public Piece {
public:
    Rook(char color, int row, int col) : Piece(color, row, col) {}

    string getSymbol() const override {
        return (color == 'W') ? "WR" : "BR";
    }

    bool isValidMove(int toRow, int toCol, Piece* const board[8][8]) const override {
        // Must move in a straight line (same row OR same column)
        if (row != toRow && col != toCol) return false;

        // Check path is clear
        if (row == toRow) {
            // Horizontal move
            int step = (toCol > col) ? 1 : -1;
            for (int c = col + step; c != toCol; c += step)
                if (board[row][c] != nullptr) return false;
        }
        else {
            // Vertical move
            int step = (toRow > row) ? 1 : -1;
            for (int r = row + step; r != toRow; r += step)
                if (board[r][col] != nullptr) return false;
        }

        // Destination: empty or enemy
        return (board[toRow][toCol] == nullptr ||
            board[toRow][toCol]->getColor() != color);
    }
};

// ============================================================
//  KNIGHT
// ============================================================
class Knight : public Piece {
public:
    Knight(char color, int row, int col) : Piece(color, row, col) {}

    string getSymbol() const override {
        return (color == 'W') ? "WN" : "BN";
    }

    bool isValidMove(int toRow, int toCol, Piece* const board[8][8]) const override {
        int dr = abs(toRow - row);
        int dc = abs(toCol - col);

        // L-shape: 2+1 or 1+2
        if (!((dr == 2 && dc == 1) || (dr == 1 && dc == 2)))
            return false;

        // Knights jump over pieces – only check destination
        return (board[toRow][toCol] == nullptr ||
            board[toRow][toCol]->getColor() != color);
    }
};

// ============================================================
//  BISHOP
// ============================================================
class Bishop : public Piece {
public:
    Bishop(char color, int row, int col) : Piece(color, row, col) {}

    string getSymbol() const override {
        return (color == 'W') ? "WB" : "BB";
    }

    bool isValidMove(int toRow, int toCol, Piece* const board[8][8]) const override {
        int dr = abs(toRow - row);
        int dc = abs(toCol - col);

        // Must be diagonal
        if (dr != dc) return false;

        // Check path is clear
        int rowStep = (toRow > row) ? 1 : -1;
        int colStep = (toCol > col) ? 1 : -1;
        int r = row + rowStep, c = col + colStep;
        while (r != toRow && c != toCol) {
            if (board[r][c] != nullptr) return false;
            r += rowStep;
            c += colStep;
        }

        return (board[toRow][toCol] == nullptr ||
            board[toRow][toCol]->getColor() != color);
    }
};

// ============================================================
//  QUEEN  (combines Rook + Bishop movement)
// ============================================================
class Queen : public Piece {
public:
    Queen(char color, int row, int col) : Piece(color, row, col) {}

    string getSymbol() const override {
        return (color == 'W') ? "WQ" : "BQ";
    }

    bool isValidMove(int toRow, int toCol, Piece* const board[8][8]) const override {
        int dr = abs(toRow - row);
        int dc = abs(toCol - col);

        bool straightLine = (row == toRow || col == toCol);
        bool diagonal = (dr == dc);

        if (!straightLine && !diagonal) return false;

        // Check path
        int rowStep = (toRow == row) ? 0 : (toRow > row ? 1 : -1);
        int colStep = (toCol == col) ? 0 : (toCol > col ? 1 : -1);

        int r = row + rowStep, c = col + colStep;
        while (r != toRow || c != toCol) {
            if (board[r][c] != nullptr) return false;
            r += rowStep;
            c += colStep;
        }

        return (board[toRow][toCol] == nullptr ||
            board[toRow][toCol]->getColor() != color);
    }
};

// ============================================================
//  KING
// ============================================================
class King : public Piece {
public:
    King(char color, int row, int col) : Piece(color, row, col) {}

    string getSymbol() const override {
        return (color == 'W') ? "WK" : "BK";
    }

    bool isValidMove(int toRow, int toCol, Piece* const board[8][8]) const override {
        int dr = abs(toRow - row);
        int dc = abs(toCol - col);

        // One step in any direction
        if (dr > 1 || dc > 1) return false;
        if (dr == 0 && dc == 0) return false;

        return (board[toRow][toCol] == nullptr ||
            board[toRow][toCol]->getColor() != color);
    }
};

#endif
