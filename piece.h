#pragma once
#ifndef PIECE_H
#define PIECE_H
#include <string>
using namespace std;
//  BASE CLASS: Piece
//  Demonstrates: Encapsulation, Inheritance, Polymorphism

class Piece {
protected:
    char color;     
    int  row, col;   
    bool captured;

public:
    // Constructor
    Piece(char color, int row, int col)
        : color(color), row(row), col(col), captured(false) {}

    // Virtual destructor (important for polymorphism)
    virtual ~Piece() {}

    // Pure virtual functions (Polymorphism)
    // Each derived class MUST implement its own move validation
    virtual bool isValidMove(int toRow, int toCol,
        Piece* const board[8][8]) const = 0;

    // Returns symbol for display (e.g., "WK" = White King)
    virtual string getSymbol() const = 0;

    // ---- Getters (Encapsulation) ----
    char getColor() const { 
        return color; 
    }
    int  getRow() const {
        return row; 
    }
    int  getCol() const {
        return col;
    }
    bool isCaptured() const {
        return captured;
    }

    // ---- Setters ----
    void setPosition(int r, int c) { 
        row = r; 
        col = c; 
    }
    void setCaptured(bool val) { 
        captured = val; 
    }
};

#endif
