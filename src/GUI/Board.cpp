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

#include "GUI/Board.hpp"
#include "GUI/Promotion.hpp"
#include <unistd.h>

static const uint8_t NoFigure = NbPieces + 1u;

Board::Board(Application &application, Rules &rules, Resources &resources, IPlayer **players)
  : GUI(application),
    m_resources(resources),
    m_rules(rules),
    m_players(players)
{
  ungrabFigure();
  m_mouse = sf::Vector2f(sf::Mouse::getPosition(window()));
  loadPosition(m_rules.m_board);
  m_thread = std::thread(&Board::play, this);
}

Board::~Board()
{
  m_players[m_rules.m_side]->abort();
  if (m_thread.joinable())
    {
      m_thread.join();
    }
}

void Board::play()
{
  uint8_t failures = 0u;
  Status previous_status = m_rules.m_status;

  usleep(100000);
  while (running())
    {
      // End of game ?
      if ((Status::Playing != m_rules.m_status) && (previous_status != m_rules.m_status))
        {
          std::cout << "End of the game: " << m_rules.m_status << " !!!" << std::endl;
          previous_status = m_rules.m_status;
          continue ;
        }
      previous_status = m_rules.m_status;

      // Get the player move
      std::string move = m_players[m_rules.m_side]->play();
      if (move == IPlayer::error)
        {
          if (!running())
            return ;

          ++failures;
          std::cout << move << " " << failures << std::endl << std::endl;
          if (failures > 7)
            {
              //m_rules.m_status = Status::InternalError;
              continue ;
            }
        }
      else
        {
          failures = 0;
          std::cout << Move(move) << std::endl << std::endl;
        }

      // Ask the GUI board to animated the move of the non-human player
      // and wait for the end of the animation.
      // Note: Human players use the mouse for playing so the figure has
      // already been moved.
      if (HumanPlayer != m_players[m_rules.m_side]->type()) // FIXME && using GUI
        {
          std::cout << "ici " <<  m_players[m_rules.m_side]->type() << std::endl;
          {
            MuxGuard g(m_lock);
            m_opponent_move = move;
          }
          while (!m_animating)
            ;
          while (m_animating)
            ;
        }

      // After the GUI animation
      m_rules.applyMove(move);

      // Debug
      if (m_rules.m_status == Status::Playing)
        {
          std::cout << m_rules.m_board
                    << std::endl
                    << m_rules.m_side
                    << " are thinking ... "
                    << std::flush;
        }
      else
        {
          std::cout << m_rules.status() << std::endl;
        }
    }
}

void Board::ungrabFigure()
{
  m_grabbed = NoFigure;
}

bool Board::grabbedFigure() const
{
  return NoFigure != m_grabbed;
}

void Board::loadPosition(chessboard const& board)
{
  uint8_t fig = 0;

  for (uint8_t ij = 0u; ij < NbSquares; ++ij)
    {
      const Piece p = board[ij];
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

Square Board::getSquare(sf::Vector2f const& mouse) const
{
  // Get the square from mouse position
  int x = (int) ((mouse.x - conf::dim::border) / ((float) conf::dim::figure));
  int y = (int) ((mouse.y - conf::dim::border) / ((float) conf::dim::figure));

  // Outside the chessboard ?
  if ((x < 0) || (x > 7) || (y < 0) || (y > 7))
    return Square::OOB;

  return static_cast<Square>(8 * y + x);
}

Piece const& Board::getPiece(sf::Vector2f const& mouse) const
{
  Square sq = getSquare(mouse);

  if (Square::OOB == sq)
    return NoPiece;

  return m_rules.m_board[sq];
}

bool Board::takeFigure(sf::Vector2f const& mouse)
{
  // No end game (checkmate, stalemate ...)
  if (Status::Playing != m_rules.m_status)
    return false;

  // The player has already grab a piece
  if (grabbedFigure())
    return false;

  // Grab a piece ?
  Square sq = getSquare(mouse);
  if (Square::OOB == sq)
    return false;

  // Do not grab a piece from opposite color
  if (m_rules.m_side != m_rules.m_board[sq].color)
    return false;

  // Find which figure is in the mouse cursor
  for (uint8_t fig = 0u; fig < NbPieces; ++fig)
    {
      if (m_resources.figures[fig].getGlobalBounds().contains(mouse.x, mouse.y))
        {
          m_grabbed = fig;
          m_from = sq;
          m_to = Square::OOB;
          break ;
        }
    }

  // Figure has been taken
  assert(grabbedFigure());
  return true;
}

bool Board::releaseFigure(sf::Vector2f const& mouse)
{
  // End of the game ?
  if (Status::Playing != m_rules.m_status)
    return false;

  // The player has already grab a piece
  if (!grabbedFigure())
    return false;

  // Outside the board dimension ?
  uint8_t m_to = getSquare(mouse);
  if (Square::OOB == m_to)
    return false;

  // Player put the piece back to its initial position:
  // ungrabFigure it to its initial position.
  if (m_from == m_to)
    {
      m_resources.figures[m_grabbed].setPosition(
          conf::dim::border + conf::dim::figure * COL(m_from),
          conf::dim::border + conf::dim::figure * ROW(m_from));
      ungrabFigure();
      return false;
    }

  // Release on its own piece
  if (m_rules.m_side == m_rules.m_board[m_to].color)
    return false;

  // Create the note move
  m_move = toStrMove(m_from, m_to);

  // Check pawn promotion and if a valif pawn promotion
  uint8_t row = ROW(m_to);
  if ((7 == row) || (0 == row))
    {
      Piece p = m_rules.m_board[m_from];
      if (PieceType::Pawn == p.type)
        {
          row = ROW(m_from);
          if (((Color::White == p.color) && (1 == row)) ||
              ((Color::Black == p.color) && (6 == row)))
            {
              // Pop up a new window for selecting the promoted piece
              Piece promote = NoPiece;
              m_application.loop(new Promotion(m_application, m_resources, promote, m_rules.m_side));

              // and complete the next move
              m_move += piece2char(promote);
            }
        }
    }

  // Accept or refuse the move
  if (!m_rules.isValidMove(m_move))
    return false;

  ungrabFigure();
  reinterpret_cast<Human*>(m_players[m_rules.m_side])->notified(m_move);
  return true;
}

bool Board::running()
{
  //m_running_thread = window().isOpen();
  return m_running_thread;
}

sf::Vector2f Board::toCoord(const char a, const char b) const
{
  return sf::Vector2f(conf::dim::border, conf::dim::border)
    + sf::Vector2f((int(a) - 'a') * conf::dim::figure,
                   ('8' - int(b)) * conf::dim::figure);
}

//! \param move the newly played move in format like "e2e4".
//! \note: the move shall be valid !
void Board::animate(const std::string& move)
{
  const sf::Vector2f from(toCoord(move[0], move[1]));
  const sf::Vector2f to(toCoord(move[2], move[3]));
  uint8_t taken_piece = NbPieces;

  for (uint8_t fig = 0u; fig < NbPieces; ++fig)
    {
      if (m_resources.figures[fig].getGlobalBounds().contains(from.x, from.y))
        {
          taken_piece = fig;
        }
    }

  // Assert piece taken (else that would mean that
  // given move was illegal)
  assert(taken_piece != NbPieces && "Illegal move");

  // Smooth animation
  const sf::Vector2f p = to - from;
  const uint32_t smooth = 50;
  for (uint32_t k = 0; k < smooth; ++k)
    {
      m_resources.figures[taken_piece].move(p.x / smooth,
                                            p.y / smooth);
      draw(0.0f);
    }
}

void Board::draw(float const /*dt*/)
{
  sf::Vector2f delta(-conf::dim::border, -conf::dim::border);

  // Set the grabbed figure to mouse cursor
  if (grabbedFigure())
    {
      delta += m_mouse - m_resources.figures[m_grabbed].getPosition();
      m_resources.figures[m_grabbed].move(delta);
    }

  // Draw the chessboard
  window().draw(m_resources.board);

  // Draw figures
  for (uint8_t i = 0u; i < NbPieces; ++i)
    window().draw(m_resources.figures[i]);

  // Swap buffer
  window().display();
}

void Board::update(float const /*dt*/)
{
  if (!m_updated)
    m_updated = true;

  loadPosition(m_rules.m_board);

  std::string move;
  {
    MuxGuard g(m_lock);
    if (m_opponent_move.empty())
      return ;
    move = m_opponent_move;
    m_opponent_move.clear();
  }
  m_animating = true;
  animate(move);
  m_animating = false;
}

void Board::handleInput()
{
  sf::Event event;

  m_mouse = sf::Vector2f(sf::Mouse::getPosition(window()));

  while (window().pollEvent(event))
    {
      switch (event.type)
        {
        case sf::Event::Closed:
          std::cout << std::endl << "Halting ChessNeuNeu ..." << std::endl;
          m_running_thread = false;
          while (window().pollEvent(event))
            ;
          m_players[m_rules.m_side]->abort();
          //window().close();
          break;

        case sf::Event::MouseButtonPressed:
          if (event.mouseButton.button == sf::Mouse::Left)
            {
              takeFigure(m_mouse);
            }
          else if (event.mouseButton.button == sf::Mouse::Right)
            {
              ungrabFigure();
              loadPosition(m_rules.m_board);
            }
          break;

        case sf::Event::MouseButtonReleased:
          m_updated = releaseFigure(m_mouse);
          if (m_updated)
            {
              //m_rules.applyMove(m_move); // FIXME: possible without game thread
              //loadPosition(m_rules.m_board);
            }
          break;

        case sf::Event::KeyPressed:
          if (event.key.code == sf::Keyboard::BackSpace)
            {
              m_rules.revertLastMove();
              loadPosition(m_rules.m_board);
            }
          else if (event.key.code == sf::Keyboard::Escape)
            {
              std::cout << std::endl << "Halting ChessNeuNeu ..." << std::endl;
              m_running_thread = false;
              while (window().pollEvent(event))
                ;
              m_players[m_rules.m_side]->abort();
              //window().close();
            }
          break;

        default:
          break;
        }
    }
}
