#include "Chess_ui.h"
#include "Pieces.h"
#include <iostream>

// ============================================================
//  Constructor
// ============================================================
ChessUI::ChessUI()
    : currentTurn('W'),
    gameOver(false),
    selectedRow(-1), selectedCol(-1),
    isPieceSelected(false),
    statusMessage("White's Turn")
{
    // ── Window ────────────────────────────────────────────────
    window.create(sf::VideoMode({ (unsigned)(tileSize * 8 + sidebarWidth),
                                  (unsigned)(tileSize * 8) }),
        "Chess Game");
    window.setFramerateLimit(60);

    // ── Font (tries project dir first, then system fonts) ─────
    if (!font.openFromFile("arial.ttf"))
        if (!font.openFromFile("D:/Chess Game/arial.ttf"))
            if (!font.openFromFile("C:/Windows/Fonts/arial.ttf"))
                if (!font.openFromFile("C:/Windows/Fonts/calibri.ttf"))
                    std::cerr << "WARNING: Could not load any font.\n";

    // ── Textures — absolute path so they always load ──────────
    const std::string BASE = "D:/Chess Game/Assests/Pieces/";

    auto loadTex = [&](std::map<std::string, sf::Texture>& map,
        const std::string& key,
        const std::string& file) {
            if (!map[key].loadFromFile(BASE + file))
                std::cerr << "Failed to load: " << BASE + file << "\n";
        };

    // Black pieces
    loadTex(blackTextures, "BK", "black_king.jpeg");
    loadTex(blackTextures, "BQ", "black_queen.jpeg");
    loadTex(blackTextures, "BR", "black_rook.jpeg");
    loadTex(blackTextures, "BB", "black_bishop.jpeg");
    loadTex(blackTextures, "BN", "black_knight.jpeg");
    loadTex(blackTextures, "BP", "black_pawn.jpeg");

    // White pieces
    loadTex(whiteTextures, "WK", "white_king.jpeg");
    loadTex(whiteTextures, "WQ", "white_queen.jpeg");
    loadTex(whiteTextures, "WR", "white_rook.jpeg");
    loadTex(whiteTextures, "WB", "white_bishop.jpeg");
    loadTex(whiteTextures, "WN", "white_knight.jpeg");
    loadTex(whiteTextures, "WP", "white_pawn.jpeg");

    // ── Board ─────────────────────────────────────────────────
    board.initializeBoard();
}

// ============================================================
//  run() — main game loop
// ============================================================
void ChessUI::run() {
    while (window.isOpen()) {

        // ── Events ────────────────────────────────────────────
        while (const std::optional<sf::Event> event = window.pollEvent()) {

            if (event->is<sf::Event::Closed>())
                window.close();

            if (!gameOver) {
                if (const auto* click =
                    event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (click->button == sf::Mouse::Button::Left)
                        handleMouseClick(click->position.x, click->position.y);
                }
            }
        }

        // ── Draw ──────────────────────────────────────────────
        window.clear(sf::Color(20, 20, 20));
        drawBoard();
        drawPieces();
        drawSidebar();
        window.display();
    }
}

// ============================================================
//  drawBoard()
// ============================================================
void ChessUI::drawBoard() {
    const sf::Color WHITE_SQ(255, 255, 255);      // pure white squares
    const sf::Color BLACK_SQ(30, 30, 30);       // near-black squares
    const sf::Color CHECKMATE_RED(200, 30, 30);   // red for checkmated king
    const sf::Color SEL_FILL(60, 40, 0);          // dark fill for selected tile

    // Find the checkmated king square (if game over by checkmate)
    int checkmateKingRow = -1, checkmateKingCol = -1;
    if (gameOver) {
        // Find the king of the loser (currentTurn holds the winner after switchTurn in updateStatus)
        // We stored the winner so the loser is the opposite
        char loser = (currentTurn == 'W') ? 'B' : 'W';
        for (int r = 0; r < 8; r++)
            for (int c = 0; c < 8; c++) {
                Piece* p = board.getPiece(r, c);
                if (p && p->getColor() == loser &&
                    p->getSymbol().substr(1) == "K") {
                    checkmateKingRow = r;
                    checkmateKingCol = c;
                }
            }
    }

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            sf::RectangleShape tile(sf::Vector2f((float)tileSize, (float)tileSize));
            tile.setPosition({ (float)(c * tileSize), (float)(r * tileSize) });

            // Checkmate king square → red
            if (r == checkmateKingRow && c == checkmateKingCol) {
                tile.setFillColor(CHECKMATE_RED);
            }
            // Selected piece square → dark fill + bright yellow border
            else if (isPieceSelected && r == selectedRow && c == selectedCol) {
                tile.setFillColor(SEL_FILL);
                tile.setOutlineColor(sf::Color(218, 165, 0));  // dark golden yellow
                tile.setOutlineThickness(4.f);
            }
            else {
                tile.setFillColor((r + c) % 2 == 0 ? WHITE_SQ : BLACK_SQ);
                tile.setOutlineThickness(0.f);
            }

            window.draw(tile);
        }
    }

    // Rank & file labels
    if (font.getInfo().family.empty()) return;
    for (int i = 0; i < 8; i++) {
        // Rank numbers (8..1) — color alternates to stay visible on both square colors
        sf::Text rank(font, std::to_string(8 - i), 12);
        rank.setFillColor((i % 2 == 0) ? sf::Color(30, 30, 30) : sf::Color(220, 220, 220));
        rank.setPosition({ 3.f, (float)(i * tileSize) + 3.f });
        window.draw(rank);

        // File letters (a..h)
        sf::Text file(font, std::string(1, 'a' + i), 12);
        file.setFillColor((i % 2 == 0) ? sf::Color(30, 30, 30) : sf::Color(220, 220, 220));
        file.setPosition({ (float)(i * tileSize + tileSize - 14),
                           (float)(tileSize * 8 - 16) });
        window.draw(file);
    }
}

// ============================================================
//  drawPieces()
// ============================================================
void ChessUI::drawPieces() {
    const float PAD = 5.f;
    const float TARGET = (float)tileSize - PAD * 2;

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            Piece* p = board.getPiece(r, c);
            if (!p) continue;

            std::string sym = p->getSymbol();   // e.g. "WK", "BP"

            // Look up in the correct map
            sf::Texture* tex = nullptr;
            if (whiteTextures.count(sym))
                tex = &whiteTextures[sym];
            else if (blackTextures.count(sym))
                tex = &blackTextures[sym];

            if (!tex || tex->getSize().x == 0) continue;

            sf::Sprite sprite(*tex);
            sf::Vector2u sz = tex->getSize();
            sprite.setScale({ TARGET / sz.x, TARGET / sz.y });
            sprite.setPosition({ (float)(c * tileSize) + PAD,
                                  (float)(r * tileSize) + PAD });
            window.draw(sprite);
        }
    }
}

// ============================================================
//  drawSidebar()
// ============================================================
void ChessUI::drawSidebar() {
    const float SX = (float)(tileSize * 8);        // sidebar left edge
    const float SW = (float)sidebarWidth;
    const float SH = (float)(tileSize * 8);

    // Background
    sf::RectangleShape bg({ SW, SH });
    bg.setPosition({ SX, 0.f });
    bg.setFillColor(sf::Color(30, 30, 30));
    window.draw(bg);

    if (font.getInfo().family.empty()) return;

    // ── Title ────────────────────────────────────────────────
    sf::Text title(font, "CHESS", 30);
    title.setFillColor(sf::Color::White);
    title.setPosition({ SX + 60.f, 15.f });
    window.draw(title);

    // ── Turn box ─────────────────────────────────────────────
    bool whiteTurn = (currentTurn == 'W');
    sf::RectangleShape turnBox({ SW - 20.f, 44.f });
    turnBox.setPosition({ SX + 10.f, 65.f });
    turnBox.setFillColor(whiteTurn ? sf::Color(240, 240, 220)
        : sf::Color(40, 40, 40));
    turnBox.setOutlineColor(sf::Color(150, 150, 150));
    turnBox.setOutlineThickness(1.f);
    window.draw(turnBox);

    sf::Text turnText(font, whiteTurn ? "WHITE'S TURN" : "BLACK'S TURN", 18);
    turnText.setFillColor(whiteTurn ? sf::Color::Black : sf::Color::White);
    turnText.setPosition({ SX + 20.f, 74.f });
    window.draw(turnText);

    // ── Status message ────────────────────────────────────────
    sf::Text status(font, statusMessage, 16);
    status.setFillColor(sf::Color(255, 220, 60));
    status.setPosition({ SX + 10.f, 125.f });
    window.draw(status);

    // ── CHECK banner (only shown when NOT game over) ──────────
    if (!gameOver && board.isInCheck(currentTurn)) {
        sf::RectangleShape checkBox({ SW - 20.f, 40.f });
        checkBox.setPosition({ SX + 10.f, 150.f });
        checkBox.setFillColor(sf::Color(180, 30, 30));
        checkBox.setOutlineColor(sf::Color(255, 100, 100));
        checkBox.setOutlineThickness(2.f);
        window.draw(checkBox);

        sf::Text checkTxt(font, "  !! CHECK !!", 20);
        checkTxt.setFillColor(sf::Color::White);
        checkTxt.setStyle(sf::Text::Bold);
        checkTxt.setPosition({ SX + 18.f, 158.f });
        window.draw(checkTxt);
    }

    // ── CHECKMATE / GAME OVER banner ─────────────────────────
    if (gameOver) {
        // Big red background box
        sf::RectangleShape overBox({ SW - 20.f, 70.f });
        overBox.setPosition({ SX + 10.f, 150.f });
        overBox.setFillColor(sf::Color(210, 20, 20));      // vivid red
        overBox.setOutlineColor(sf::Color(255, 80, 80));   // lighter red border
        overBox.setOutlineThickness(3.f);
        window.draw(overBox);

        sf::Text overTxt1(font, "CHECKMATE!", 22);
        overTxt1.setFillColor(sf::Color::White);
        overTxt1.setStyle(sf::Text::Bold);
        overTxt1.setPosition({ SX + 18.f, 156.f });
        window.draw(overTxt1);

        sf::Text overTxt2(font, statusMessage, 14);
        overTxt2.setFillColor(sf::Color(255, 220, 220));
        overTxt2.setPosition({ SX + 14.f, 192.f });
        window.draw(overTxt2);
    }

    // ── Legend ───────────────────────────────────────────────
    sf::Text leg(font, "Pieces:", 14);
    leg.setFillColor(sf::Color(180, 180, 180));
    leg.setPosition({ SX + 10.f, 210.f });
    window.draw(leg);

    const char* legend[] = {
        "K = King",   "Q = Queen", "R = Rook",
        "B = Bishop", "N = Knight","P = Pawn"
    };
    for (int i = 0; i < 6; i++) {
        sf::Text l(font, legend[i], 13);
        l.setFillColor(sf::Color::White);
        l.setPosition({ SX + 10.f, 230.f + i * 20.f });
        window.draw(l);
    }

    // ── How to play ───────────────────────────────────────────
    sf::Text how(font, "How to play:", 14);
    how.setFillColor(sf::Color(180, 180, 180));
    how.setPosition({ SX + 10.f, 365.f });
    window.draw(how);

    sf::Text h1(font, "1. Click your piece", 13);
    h1.setFillColor(sf::Color::White);
    h1.setPosition({ SX + 10.f, 385.f });
    window.draw(h1);

    sf::Text h2(font, "2. Click destination", 13);
    h2.setFillColor(sf::Color::White);
    h2.setPosition({ SX + 10.f, 403.f });
    window.draw(h2);

    sf::Text h3(font, "Selected = yellow", 13);
    h3.setFillColor(sf::Color(252, 220, 60));
    h3.setPosition({ SX + 10.f, 421.f });
    window.draw(h3);
}

// ============================================================
//  handleMouseClick()
// ============================================================
void ChessUI::handleMouseClick(int x, int y) {
    // Ignore clicks in sidebar
    if (x >= tileSize * 8) return;

    int col = x / tileSize;
    int row = y / tileSize;

    // Guard bounds
    if (row < 0 || row > 7 || col < 0 || col > 7) return;

    if (!isPieceSelected) {
        // ── First click: select a piece ───────────────────────
        Piece* p = board.getPiece(row, col);
        if (p && p->getColor() == currentTurn) {
            selectedRow = row;
            selectedCol = col;
            isPieceSelected = true;
            statusMessage = "Piece selected";
        }
        else {
            statusMessage = "Pick your own piece!";
        }

    }
    else {
        // ── Second click: move or deselect ────────────────────
        if (row == selectedRow && col == selectedCol) {
            // Clicked same square → cancel
            isPieceSelected = false;
            statusMessage = (currentTurn == 'W') ? "White's Turn"
                : "Black's Turn";
            return;
        }

        if (tryMove(selectedRow, selectedCol, row, col)) {
            isPieceSelected = false;
            switchTurn();
            updateStatusAfterMove();
        }
        else {
            // Invalid move — let player reselect
            isPieceSelected = false;
        }
    }
}

// ============================================================
//  tryMove() — identical logic to game.cpp::applyMove()
//  Returns true only if the move is fully legal.
// ============================================================
bool ChessUI::tryMove(int fr, int fc, int tr, int tc) {
    Piece* piece = board.getPiece(fr, fc);
    if (!piece || piece->getColor() != currentTurn) {
        statusMessage = "Not your piece!";
        return false;
    }

    // Build grid snapshot for isValidMove
    Piece* tempGrid[8][8];
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++)
            tempGrid[r][c] = board.getPiece(r, c);

    if (!piece->isValidMove(tr, tc, tempGrid)) {
        statusMessage = "Invalid move!";
        return false;
    }

    // Simulate move and check king safety
    Piece* simGrid[8][8];
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++)
            simGrid[r][c] = tempGrid[r][c];

    simGrid[tr][tc] = simGrid[fr][fc];
    simGrid[fr][fc] = nullptr;
    if (simGrid[tr][tc])
        simGrid[tr][tc]->setPosition(tr, tc);

    // Find our King in simulated board
    int kr = -1, kc = -1;
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++)
            if (simGrid[r][c] &&
                simGrid[r][c]->getColor() == currentTurn &&
                simGrid[r][c]->getSymbol().substr(1) == "K")
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

    // Undo temp position change
    if (simGrid[tr][tc])
        simGrid[tr][tc]->setPosition(fr, fc);

    if (leavesInCheck) {
        statusMessage = "Exposes your King!";
        return false;
    }

    // Commit the real move
    Piece* captured = board.movePiece(fr, fc, tr, tc);
    if (captured) delete captured;

    return true;
}

// ============================================================
//  switchTurn()
// ============================================================
void ChessUI::switchTurn() {
    currentTurn = (currentTurn == 'W') ? 'B' : 'W';
}

// ============================================================
//  updateStatusAfterMove() — check/checkmate/stalemate
// ============================================================
void ChessUI::updateStatusAfterMove() {
    if (board.isCheckmate(currentTurn)) {
        // Switch back to name the winner
        switchTurn();
        std::string winner = (currentTurn == 'W') ? "White" : "Black";
        statusMessage = winner + " wins! Checkmate!";
        gameOver = true;
    }
    else if (board.isStalemate(currentTurn)) {
        statusMessage = "Stalemate! It's a draw!";
        gameOver = true;
    }
    else if (board.isInCheck(currentTurn)) {
        statusMessage = "CHECK!";
    }
    else {
        statusMessage = (currentTurn == 'W') ? "White's Turn" : "Black's Turn";
    }
}
