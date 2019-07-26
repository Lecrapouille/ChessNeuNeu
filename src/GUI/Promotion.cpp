//=====================================================================
// ChessNeuNeu: Non serious chess engine for learning neural networks.
// Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
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

Promotion::Promotion(Application& application, Resources &resources, Piece &taken_piece, Color color)
  : GUI(application), m_resources(resources), m_taken_piece(taken_piece)
{
  assert(Color::NoColor != color);

  // Load a dummy board with major pieces.
  m_board = Chessboard::Empty;
  if (Color::White == color)
    {
      m_board[sqC4] = WhiteRook;
      m_board[sqD4] = WhiteKing;
      m_board[sqE4] = WhiteBishop;
      m_board[sqF4] = WhiteQueen;
    }
  else if (Color::Black == color)
    {
      m_board[sqC4] = BlackRook;
      m_board[sqD4] = BlackKing;
      m_board[sqE4] = BlackBishop;
      m_board[sqF4] = BlackQueen;
    }
  loadPosition();
}

Promotion::~Promotion()
{
}

void Promotion::loadPosition()
{
  uint8_t fig = 0;

  for (uint8_t ij = sqC4; ij <= sqF4; ++ij)
    {
      Piece const p = m_board[ij];
      if (p.type() == PieceType::Empty)
        continue;

      // Don't want the sprite to use the entire texture
      int32_t const x = p.m_type - 1;
      int32_t const y = p.m_color;
      m_resources.figures[fig].setTextureRect(
             sf::IntRect(conf::dim::figure * x,
                         conf::dim::figure * y,
                         conf::dim::figure,
                         conf::dim::figure));

      // Absolute position
      m_resources.figures[fig].setPosition(
             conf::dim::border + conf::dim::figure * float(COL(ij)),
             conf::dim::border + conf::dim::figure * float(ROW(ij)));
      ++fig;
      assert(fig <= NbPieces);
    }

  // Hide unused pieces
  for (; fig < NbPieces; ++fig)
    m_resources.figures[fig].setPosition(-1000, -1000);
}

Piece const& Promotion::getPiece(sf::Vector2f const& mouse) const
{
  // Get the square from mouse position
  int const x = static_cast<int>((mouse.x - conf::dim::border) / (float(conf::dim::figure)));
  int const y = static_cast<int>((mouse.y - conf::dim::border) / (float(conf::dim::figure)));

  // Outside the chessboard ?
  if ((x < 0) || (x >= NbCols) || (y < 0) || (y >= NbRows))
    return NoPiece;

  return m_board[static_cast<size_t>(y * NbRows + x)];
}

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

void Promotion::draw(float const /*dt*/)
{
  // Draw the chessboard
  window().draw(m_resources.board);

  // Draw figures
  for (uint8_t i = 0u; i < NbPieces; ++i)
    window().draw(m_resources.figures[i]);

  // Swap buffer
  window().display();
}

void Promotion::update(float const /*dt*/)
{
}

bool Promotion::running()
{
  return window().isOpen() &&
    (PieceType::Empty == m_taken_piece.type());
}

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
          if (PieceType::Empty != taken_piece.type())
            {
              std::cout << "Promotion::Promotion: " << taken_piece << std::endl;
              m_taken_piece = taken_piece;
            }
          break;

        case sf::Event::LostFocus:
        case sf::Event::GainedFocus:
        case sf::Event::TextEntered:
        case sf::Event::KeyReleased:
        case sf::Event::MouseWheelMoved:
        case sf::Event::MouseWheelScrolled:
        case sf::Event::MouseMoved:
        case sf::Event::MouseEntered:
        case sf::Event::MouseLeft:
        case sf::Event::JoystickButtonPressed:
        case sf::Event::JoystickButtonReleased:
        case sf::Event::JoystickMoved:
        case sf::Event::JoystickConnected:
        case sf::Event::JoystickDisconnected:
        case sf::Event::TouchBegan:
        case sf::Event::TouchMoved:
        case sf::Event::TouchEnded:
        case sf::Event::SensorChanged:
        case sf::Event::Count:
        case sf::Event::Resized:
        case sf::Event::KeyPressed:
        case sf::Event::MouseButtonReleased:
        default:
          break;
        }
    }
}
