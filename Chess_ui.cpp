//// ============================================================
////  chess_ui.cpp  —  Raylib Chess UI  (Implementation)
////
////  WHAT RAYLIB GIVES US:
////    InitWindow()         → opens the OS window
////    WindowShouldClose()  → true when user presses ✕ or ESC
////    BeginDrawing()       → start a frame
////    EndDrawing()         → finish frame, push to screen
////    ClearBackground()    → fill screen with a color
////    DrawRectangle()      → filled rectangle
////    DrawRectangleLines() → outline rectangle
////    DrawCircle()         → filled circle
////    DrawText()           → ASCII text
////    MeasureText()        → pixel width of a string (for centering)
////    GetMouseX/Y()        → current cursor position
////    IsMouseButtonPressed()→ click detection
////    SetTargetFPS()       → cap frame rate (saves CPU)
////    CloseWindow()        → clean up
//// ============================================================
//#include "raylib.h"
//#include "chess_ui.h"
//#include "Pieces.h"
//#include <string>
//using namespace std;
//
//// ============================================================
////  Constructor
//// ============================================================
//ChessUI::ChessUI()
//    : currentTurn('W'),
//    gameOver(false),
//    selRow(-1), selCol(-1),
//    hasSel(false),
//    statusMsg("White's turn - click a piece")
//{
//    board.initializeBoard();
//}
//
//// ============================================================
////  run()  —  THE GAME LOOP
////
////  This is the heart of any Raylib program:
////
////      InitWindow(width, height, title);
////      SetTargetFPS(60);
////
////      while (!WindowShouldClose()) {   ← loop until ✕ or ESC
////          // 1. Process input
////          // 2. Update state
////          // 3. Draw
////          BeginDrawing();
////              ...draw calls...
////          EndDrawing();
////      }
////
////      CloseWindow();
//// ============================================================
//void ChessUI::run() {
//    InitWindow(WIN_W, WIN_H, "Chess Game — Raylib");
//    SetTargetFPS(60);   // smooth without burning the CPU
//
//    while (!WindowShouldClose()) {
//
//        // --- INPUT ---
//        // Raylib gives us mouse events outside BeginDrawing.
//        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
//            int mx = GetMouseX();
//            int my = GetMouseY();
//            if (mx < BOARD_PX)          // only if clicked on board area
//                handleClick(mx, my);
//        }
//
//        // --- DRAW ---
//        BeginDrawing();                  // must wrap ALL draw calls
//        ClearBackground({ 30, 30, 30, 255 });   // dark grey background
//        drawBoard();
//        drawPieces();
//        drawSidebar();
//        EndDrawing();
//
//        if (gameOver) break;             // exit loop after drawing final state
//    }
//
//    // Show the final position for 3 seconds before closing
//    if (gameOver) {
//        double t = GetTime();
//        while (GetTime() - t < 3.0 && !WindowShouldClose()) {
//            BeginDrawing();
//            ClearBackground({ 30, 30, 30, 255 });
//            drawBoard();
//            drawPieces();
//            drawSidebar();
//            EndDrawing();
//        }
//    }
//
//    CloseWindow();
//}
//
//// ============================================================
////  drawBoard()  —  The 64 squares
////
////  DrawRectangle(x, y, width, height, color)
////    x, y = top-left corner of the rectangle (pixels)
////    Raylib origin (0,0) is TOP-LEFT of window.
//// ============================================================
//void ChessUI::drawBoard() {
//    for (int r = 0; r < 8; r++) {
//        for (int c = 0; c < 8; c++) {
//            Color col = squareColor(r, c);
//
//            // Yellow highlight for the selected piece's square
//            if (hasSel && r == selRow && c == selCol)
//                col = { 252, 220, 60, 255 };   // bright yellow
//
//            DrawRectangle(c * TILE, r * TILE, TILE, TILE, col);
//        }
//    }
//
//    // Board border
//    DrawRectangleLines(0, 0, BOARD_PX, BOARD_PX, { 80, 50, 20, 255 });
//
//    // Rank labels (8 down to 1) on the left edge
//    for (int r = 0; r < 8; r++) {
//        DrawText(TextFormat("%d", 8 - r),
//            4,                        // x: 4px from left
//            r * TILE + 4,             // y: 4px below top of square
//            14,
//            { 100, 70, 40, 255 });
//    }
//
//    // File labels (a through h) at the bottom edge
//    for (int c = 0; c < 8; c++) {
//        DrawText(TextFormat("%c", 'a' + c),
//            c * TILE + TILE - 12,     // x: near right of square
//            BOARD_PX - 18,            // y: near bottom
//            14,
//            { 100, 70, 40, 255 });
//    }
//}
//
//// ============================================================
////  drawPieces()  —  Draw each piece as a circle + letter
////
////  We draw:
////    1. A dark outline circle
////    2. A filled circle (white for W, near-black for B)
////    3. The piece letter centered inside (K Q R B N P)
////
////  DrawCircle(centerX, centerY, radius, color)
////  DrawText(text, x, y, fontSize, color)
////  MeasureText(text, fontSize)  → pixel width (used to center text)
//// ============================================================
//void ChessUI::drawPieces() {
//    const int RADIUS = TILE / 2 - 6;   // fits inside the square
//    const int FONT_SIZE = 26;
//
//    for (int r = 0; r < 8; r++) {
//        for (int c = 0; c < 8; c++) {
//            Piece* p = board.getPiece(r, c);
//            if (!p) continue;
//
//            // Centre of this square in pixels
//            int cx = c * TILE + TILE / 2;
//            int cy = r * TILE + TILE / 2;
//
//            // Colors
//            bool isWhite = (p->getColor() == 'W');
//            Color fillColor = isWhite ? Color{ 240, 240, 220, 255 }
//            : Color{ 40,  40,  40, 255 };
//            Color ringColor = isWhite ? Color{ 20,  20,  20, 255 }
//            : Color{ 200, 200, 200, 255 };
//            Color textColor = isWhite ? Color{ 20,  20,  20, 255 }
//            : Color{ 220, 220, 220, 255 };
//
//            // Draw: shadow → ring → fill
//            DrawCircle(cx + 2, cy + 2, RADIUS, { 0, 0, 0, 80 }); // shadow
//            DrawCircle(cx, cy, RADIUS, ringColor);   // outline ring
//            DrawCircle(cx, cy, RADIUS - 4, fillColor);   // filled circle
//
//            // Piece letter (second char of symbol: WK→K, BP→P …)
//            string sym = p->getSymbol();
//            string letter(1, sym[1]);                             // e.g. "K"
//            int tw = MeasureText(letter.c_str(), FONT_SIZE);
//            DrawText(letter.c_str(),
//                cx - tw / 2,
//                cy - FONT_SIZE / 2,
//                FONT_SIZE,
//                textColor);
//        }
//    }
//}
//
//// ============================================================
////  drawSidebar()  —  Right panel: turn, status, legend, help
//// ============================================================
//void ChessUI::drawSidebar() {
//    const int SX = BOARD_PX + 10;   // sidebar X start
//    const int W = 180;             // usable width
//
//    // Title
//    DrawText("CHESS", SX + 40, 15, 28, RAYWHITE);
//    DrawLine(SX, 55, SX + W, 55, GRAY);
//
//    // ---- Current turn box ----
//    DrawText("Current Turn", SX, 65, 14, LIGHTGRAY);
//    bool whiteTurn = (currentTurn == 'W');
//    Color boxFill = whiteTurn ? Color{ 240, 240, 220, 255 }
//    : Color{ 40,  40,  40, 255 };
//    Color boxText = whiteTurn ? BLACK : WHITE;
//    DrawRectangleRounded({ (float)SX, 85, (float)W, 38 }, 0.3f, 4, boxFill);
//    DrawRectangleRoundedLines({ (float)SX, 85, (float)W, 38 }, 0.3f, 4, 2,
//        whiteTurn ? DARKGRAY : LIGHTGRAY);
//    const char* turnLabel = whiteTurn ? "WHITE" : "BLACK";
//    int tw = MeasureText(turnLabel, 22);
//    DrawText(turnLabel, SX + W / 2 - tw / 2, 93, 22, boxText);
//
//    // ---- Status message ----
//    DrawLine(SX, 135, SX + W, 135, GRAY);
//    DrawText("Status", SX, 143, 14, LIGHTGRAY);
//    // Word-wrap isn't built-in; keep messages ≤ 20 chars or split manually
//    DrawText(statusMsg.c_str(), SX, 162, 13, YELLOW);
//
//    // ---- Check / Checkmate indicator ----
//    if (!gameOver && board.isInCheck(currentTurn)) {
//        DrawRectangleRounded({ (float)SX, 190, (float)W, 36 }, 0.3f, 4, RED);
//        int cw = MeasureText("  CHECK!", 20);
//        DrawText("  CHECK!", SX + W / 2 - cw / 2, 198, 20, WHITE);
//    }
//    if (gameOver) {
//        DrawRectangleRounded({ (float)SX, 190, (float)W, 36 }, 0.3f, 4,
//            { 180, 30, 30, 255 });
//        int gw = MeasureText("GAME OVER", 18);
//        DrawText("GAME OVER", SX + W / 2 - gw / 2, 199, 18, WHITE);
//    }
//
//    // ---- Legend ----
//    DrawLine(SX, 240, SX + W, 240, GRAY);
//    DrawText("Pieces", SX, 248, 14, LIGHTGRAY);
//    const char* legend[] = { "K = King","Q = Queen","R = Rook",
//                            "B = Bishop","N = Knight","P = Pawn" };
//    for (int i = 0; i < 6; i++)
//        DrawText(legend[i], SX, 266 + i * 18, 13, RAYWHITE);
//
//    // ---- How to play ----
//    DrawLine(SX, 385, SX + W, 385, GRAY);
//    DrawText("How to play", SX, 393, 14, LIGHTGRAY);
//    DrawText("1. Click your piece", SX, 411, 12, RAYWHITE);
//    DrawText("2. Click destination", SX, 427, 12, RAYWHITE);
//    DrawText("Selected = yellow", SX, 443, 12, { 252, 220, 60, 255 });
//
//    // ---- Quit hint ----
//    DrawLine(SX, 470, SX + W, 470, GRAY);
//    DrawText("Press ESC to quit", SX, 478, 12, GRAY);
//}
//
//// ============================================================
////  squareColor()  —  Classic chess board colouring
////  (r+c) even → light cream   (r+c) odd → warm brown
//// ============================================================
//Color ChessUI::squareColor(int r, int c) const {
//    if ((r + c) % 2 == 0)
//        return { 240, 217, 181, 255 };   // cream / light
//    else
//        return { 181, 136,  99, 255 };   // brown / dark
//}
//
//// ============================================================
////  handleClick()  —  Two-click move system
////
////  Click 1:  select one of your pieces  (turns it yellow)
////  Click 2:  pick a destination square  (attempts the move)
////  Click same square twice: deselect
//// ============================================================
//void ChessUI::handleClick(int mx, int my) {
//    int clickRow = my / TILE;
//    int clickCol = mx / TILE;
//
//    // Guard: must be within board
//    if (clickRow < 0 || clickRow > 7 || clickCol < 0 || clickCol > 7) return;
//
//    if (!hasSel) {
//        // ---- First click: select ----
//        Piece* p = board.getPiece(clickRow, clickCol);
//        if (p && p->getColor() == currentTurn) {
//            selRow = clickRow;
//            selCol = clickCol;
//            hasSel = true;
//            statusMsg = "Piece selected";
//        }
//        else {
//            statusMsg = "Pick your piece!";
//        }
//    }
//    else {
//        // ---- Second click: move or deselect ----
//        if (clickRow == selRow && clickCol == selCol) {
//            // Same square → cancel selection
//            hasSel = false;
//            statusMsg = (currentTurn == 'W') ? "White's turn"
//                : "Black's turn";
//            return;
//        }
//
//        if (tryMove(selRow, selCol, clickRow, clickCol)) {
//            // Move succeeded
//            hasSel = false;
//            switchTurn();
//
//            if (board.isCheckmate(currentTurn)) {
//                switchTurn();   // switch back to show winner
//                string winner = (currentTurn == 'W') ? "White" : "Black";
//                statusMsg = winner + " wins!";
//                gameOver = true;
//            }
//            else if (board.isStalemate(currentTurn)) {
//                statusMsg = "Stalemate! Draw!";
//                gameOver = true;
//            }
//            else if (board.isInCheck(currentTurn)) {
//                statusMsg = "Check!";
//            }
//            else {
//                statusMsg = (currentTurn == 'W') ? "White's turn"
//                    : "Black's turn";
//            }
//        }
//        else {
//            // tryMove() already set statusMsg with the reason
//            hasSel = false;
//        }
//    }
//}
//
//// ============================================================
////  tryMove()  —  Validate & execute a move
////  This is the same logic as game.cpp::applyMove(), unchanged.
//// ============================================================
//bool ChessUI::tryMove(int fr, int fc, int tr, int tc) {
//    Piece* piece = board.getPiece(fr, fc);
//    if (!piece || piece->getColor() != currentTurn) {
//        statusMsg = "Not your piece!";
//        return false;
//    }
//
//    // Snapshot the board for isValidMove checks
//    Piece* tempGrid[8][8];
//    for (int r = 0; r < 8; r++)
//        for (int c = 0; c < 8; c++)
//            tempGrid[r][c] = board.getPiece(r, c);
//
//    if (!piece->isValidMove(tr, tc, tempGrid)) {
//        statusMsg = "Invalid move!";
//        return false;
//    }
//
//    // Simulate to ensure King not left in check
//    Piece* simGrid[8][8];
//    for (int r = 0; r < 8; r++)
//        for (int c = 0; c < 8; c++)
//            simGrid[r][c] = tempGrid[r][c];
//
//    simGrid[tr][tc] = simGrid[fr][fc];
//    simGrid[fr][fc] = nullptr;
//    if (simGrid[tr][tc]) simGrid[tr][tc]->setPosition(tr, tc);
//
//    // Find the King in simulated board
//    int kr = -1, kc = -1;
//    for (int r = 0; r < 8; r++)
//        for (int c = 0; c < 8; c++)
//            if (simGrid[r][c] && simGrid[r][c]->getColor() == currentTurn
//                && simGrid[r][c]->getSymbol().substr(1) == "K")
//            {
//                kr = r; kc = c;
//            }
//
//    bool leavesInCheck = false;
//    char opp = (currentTurn == 'W') ? 'B' : 'W';
//    if (kr != -1)
//        for (int r = 0; r < 8; r++)
//            for (int c = 0; c < 8; c++)
//                if (simGrid[r][c] && simGrid[r][c]->getColor() == opp)
//                    if (simGrid[r][c]->isValidMove(kr, kc, simGrid))
//                        leavesInCheck = true;
//
//    // Undo the temporary position change
//    if (simGrid[tr][tc]) simGrid[tr][tc]->setPosition(fr, fc);
//
//    if (leavesInCheck) {
//        statusMsg = "Exposes your King!";
//        return false;
//    }
//
//    // All checks passed — make the actual move
//    Piece* captured = board.movePiece(fr, fc, tr, tc);
//    if (captured) delete captured;
//
//    return true;
//}
//
//// ============================================================
//void ChessUI::switchTurn() {
//    currentTurn = (currentTurn == 'W') ? 'B' : 'W';
//}