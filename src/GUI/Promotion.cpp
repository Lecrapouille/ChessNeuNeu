//=====================================================================
// ChessNeuNeu: Non serious chess engine for learning neural networks.
// Copyright 2018 -- 2022 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of ChessNeuNeu.
//
// ChessNeuNeu is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#include "GUI/Promotion.hpp"

//------------------------------------------------------------------------------
Promotion::Promotion(Application& application, Resources &resources, Color color)
    : GUI("Promotion", application),
      m_resources(resources)
{
    // Load a dummy board with major pieces.

    m_board = Chessboard::Empty;
    m_board[sqC4] = (color == Color::Black) ? BlackRook : WhiteRook;
    m_board[sqD4] = (color == Color::Black) ? BlackKnight : WhiteKnight;
    m_board[sqE4] = (color == Color::Black) ? BlackBishop : WhiteBishop;
    m_board[sqF4] = (color == Color::Black) ? BlackQueen : WhiteQueen;
}

//------------------------------------------------------------------------------
void Promotion::activate()
{
    m_promoted_figure = NoPiece;
    loadPosition();
}

//------------------------------------------------------------------------------
void Promotion::deactivate()
{}

//------------------------------------------------------------------------------
void Promotion::loadPosition()
{
    uint8_t fig = 0;

    for (uint8_t ij = sqC4; ij <= sqF4; ++ij)
    {
        const Piece p = m_board[ij];
        if (p.type == PieceType::Empty)
            continue;

        // Don't want the sprite to use the entire texture
        const uint32_t x = p.type - 1u;
        const uint32_t y = p.color;
        m_resources.figures[fig].setTextureRect(
            sf::IntRect(conf::dim::figure * x,
                        conf::dim::figure * y,
                        conf::dim::figure,
                        conf::dim::figure));

        // Absolute position
        m_resources.figures[fig].setPosition(
            conf::dim::border + conf::dim::figure * COL(ij),
            conf::dim::border + conf::dim::figure * ROW(ij));
        ++fig;
        assert(fig <= NbPieces);
    }

    // Hide unused pieces
    for (; fig < NbPieces; ++fig)
        m_resources.figures[fig].setPosition(-1000, -1000);
}

//------------------------------------------------------------------------------
const Piece &Promotion::getPiece(const sf::Vector2f& mouse) const
{
    // Get the square from mouse position
    int x = (int) ((mouse.x - conf::dim::border) / ((float) conf::dim::figure));
    int y = (int) ((mouse.y - conf::dim::border) / ((float) conf::dim::figure));

    // Outside the chessboard ?
    if ((x < 0) || (x > 7) || (y < 0) || (y > 7))
        return NoPiece;

    return m_board[y * 8 + x];
}

//------------------------------------------------------------------------------
Piece Promotion::takeFigure()
{
    // Find which piece is in the mouse cursor
    for (uint8_t i = 0u; i < NbPieces; ++i)
    {
        if (m_resources.figures[i].getGlobalBounds().contains(m_mouse.x, m_mouse.y))
        {
            return getPiece(m_mouse);
        }
    }
    return NoPiece;
}

//------------------------------------------------------------------------------
void Promotion::draw(const float /*dt*/)
{
    // Draw the chessboard
    window().draw(m_resources.board);

    // Draw figures
    for (uint8_t i = 0u; i < NbPieces; ++i)
        window().draw(m_resources.figures[i]);

    // Swap buffer
    window().display();
}

//------------------------------------------------------------------------------
void Promotion::update(const float /*dt*/)
{}

//------------------------------------------------------------------------------
bool Promotion::isRunning()
{
    return window().isOpen() &&
            (PieceType::Empty == m_promoted_figure.type);
}

//------------------------------------------------------------------------------
void Promotion::handleInput()
{
    sf::Event event;
    Piece taken_piece = NoPiece;

    m_mouse = sf::Vector2f(sf::Mouse::getPosition(window()));

    while (window().pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            window().close();
            break;

        case sf::Event::MouseButtonPressed:
            taken_piece = takeFigure();
            if (PieceType::Empty != taken_piece.type)
            {
                std::cout << "Promotion::Promotion: " << taken_piece << std::endl;
                m_promoted_figure = taken_piece;
            }
            break;

        default:
            break;
        }
    }
}
