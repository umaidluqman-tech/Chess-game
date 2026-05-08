#pragma once
#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include <string>
using namespace std;

// ============================================================
//  GAME CLASS  –  Manages turns, input, win/loss detection
// ============================================================
class Game {
private:
    Board board;
    char  currentTurn;   // 'W' or 'B'
    bool  gameOver;

public:
    Game();

    void start();          // Main game loop

private:
    void  printTurnInfo() const;
    bool  getMove(int& fromRow, int& fromCol, int& toRow, int& toCol);
    bool  parsePosition(const string& s, int& row, int& col) const;
    bool  applyMove(int fromRow, int fromCol, int toRow, int toCol);
    void  switchTurn();
    void  printWinner() const;
};

#endif
