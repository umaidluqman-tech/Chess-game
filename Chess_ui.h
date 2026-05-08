#pragma once
#ifndef CHESS_UI_H
#define CHESS_UI_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include "Board.h"

class ChessUI {
public:
    ChessUI();
    void run();

private:
    // ── SFML objects ───────────────────────────────────────────
    sf::RenderWindow        window;
    sf::Font                font;

    // ── Piece textures (keyed by getSymbol(): "WK","BP", etc.) ─
    std::map<std::string, sf::Texture> whiteTextures;
    std::map<std::string, sf::Texture> blackTextures;

    // ── Game state ─────────────────────────────────────────────
    Board       board;
    char        currentTurn;      // 'W' or 'B'
    bool        gameOver;

    // ── Selection state ────────────────────────────────────────
    int         selectedRow, selectedCol;
    bool        isPieceSelected;

    // ── Status message shown in sidebar ────────────────────────
    std::string statusMessage;

    // ── Layout ─────────────────────────────────────────────────
    static const int tileSize = 90;   // px per square  (8*90 = 720)
    static const int sidebarWidth = 230;  // sidebar px      (720+230 = 950)

    // ── Drawing helpers ────────────────────────────────────────
    void drawBoard();
    void drawPieces();
    void drawSidebar();

    // ── Input ──────────────────────────────────────────────────
    void handleMouseClick(int x, int y);

    // ── Move logic (mirrors game.cpp — correct check detection) ─
    bool tryMove(int fr, int fc, int tr, int tc);
    void switchTurn();
    void updateStatusAfterMove();
};

#endif
