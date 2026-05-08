//#pragma once
//#ifndef CHESS_UI_H
//#define CHESS_UI_H
//
//// ============================================================
////  chess_ui.h  —  Raylib-powered Chess UI
////
////  CONCEPT: This class is a "view + controller" layer.
////  Your Board/Piece/Game LOGIC stays exactly the same.
////  We just replace the terminal display() with a Raylib window.
////
////  KEY RAYLIB IDEAS:
////    - A "game loop": while (!WindowShouldClose()) { ... }
////    - Each frame: BeginDrawing() → draw stuff → EndDrawing()
////    - Mouse input: GetMouseX/Y, IsMouseButtonPressed()
//// ============================================================
//
//#include "Board.h"     // your unchanged Board class
//#include "raylib.h"    // the one header that gives us everything
//
//#include <string>
//
//class ChessUI {
//private:
//    Board  board;
//    char   currentTurn;   // 'W' or 'B'
//    bool   gameOver;
//
//    // ---- Selection state ----
//    // When the player clicks a piece, we remember where it is.
//    // On the second click we attempt the move.
//    int  selRow, selCol;  // which square is selected
//    bool hasSel;          // is something selected right now?
//
//    // ---- Status message (shown in sidebar) ----
//    std::string statusMsg;
//
//    // ---- Layout constants ----
//    // Everything is derived from TILE so you can resize by
//    // changing just one number.
//    static const int TILE = 75;          // pixels per square
//    static const int BOARD_PX = TILE * 8;  // 600 px
//    static const int WIN_W = BOARD_PX + 200; // 800 px total
//    static const int WIN_H = BOARD_PX;       // 600 px
//
//public:
//    ChessUI();
//    void run();   // <-- call this from main()
//
//private:
//    // ---- Drawing helpers (called every frame) ----
//    void drawBoard();     // draw the 64 squares
//    void drawPieces();    // draw pieces on top of squares
//    void drawSidebar();   // right panel: turn, status, help
//
//    // ---- Input handling ----
//    void handleClick(int mouseX, int mouseY);
//
//    // ---- Game logic (mirrors game.cpp but UI-friendly) ----
//    bool tryMove(int fr, int fc, int tr, int tc);
//    void switchTurn();
//
//    // ---- Tiny helpers ----
//    Color squareColor(int r, int c) const;   // cream or brown
//};
//
//#endif