#include "Board.hpp"
#include <iostream>

Board::Board(Rules &rules, sf::RenderWindow &window)
  : m_rules(rules), // FIXME ideal si on on peut le virer
    m_window(window),
    m_piece(0u),
    m_moving_figure(false)
{
  loadTextures();
  loadPosition(m_rules.m_current_position);
}

Board::~Board()
{
}

void Board::loadTextures()
{
  bool res1 = m_textures[0].loadFromFile("data/figures.png");
  bool res2 = m_textures[1].loadFromFile("data/board.png");
  if ((false == res1) || (false == res2))
    {
      std::cerr << "Failed loading textures" << std::endl;
    }

  for (uint8_t i = 0u; i < 32u; ++i)
    {
      m_figures[i].setTexture(m_textures[0]);
    }
  m_sboard.setTexture(m_textures[1]);
}

void Board::loadPosition(const chessboard& board)
{
  uint8_t k = 0;

  for (uint8_t ij = 0u; ij < 64u; ++ij)
    {
      const Piece n = board[ij];
      if (n.type == PieceType::Empty)
        continue;

      const uint32_t x = n.type - 1u;
      const uint32_t y = n.color;
      m_figures[k].setTextureRect(sf::IntRect(config::figure_size * x,
                                              config::figure_size * y,
                                              config::figure_size,
                                              config::figure_size));
      m_figures[k].setPosition(config::figure_size * COL(ij),
                               config::figure_size * ROW(ij));
      ++k;
    }

  for (; k < 32u; ++k)
    m_figures[k].setPosition(-1000, -1000);
}

//! \param move the new valid move like "e2e4".
//! \note: the move shall be valid !
void Board::animation(const std::string& movement) //  FIXME Move au lieu de movement
{
  m_old_pos = toCoord(movement[0], movement[1]);
  m_new_pos = toCoord(movement[2], movement[3]);

  for (uint8_t i = 0u; i < 32u; ++i)
    {
      if (m_figures[i].getPosition() == m_old_pos)
        {
          m_piece = i;
        }
    }

  // Smooth animation
  for (uint32_t k = 0; k < 50; ++k)
    {
      sf::Vector2f p = m_new_pos - m_old_pos;
      m_figures[m_piece].move(p.x / 50, p.y / 50);
      draw();
    }

  noAnimation(movement);
}

void Board::noAnimation(const std::string& movement) // FIXME Move au lieu de movement
{
  sf::Vector2f oldPos = toCoord(movement[0], movement[1]);
  sf::Vector2f newPos = toCoord(movement[2], movement[3]);

  for (uint8_t i = 0u; i < 32u; ++i)
    {
      if (m_figures[i].getPosition() == newPos)
        {
          m_figures[i].setPosition(-100, -100);
        }
    }

  for (uint8_t i = 0u; i < 32u; ++i)
    {
      if (m_figures[i].getPosition() == oldPos)
        {
          m_figures[i].setPosition(newPos);
        }
    }

  m_figures[m_piece].setPosition(m_new_pos);
}

void Board::oppentMove(const std::string& next_move)
{
  animation(next_move);
  m_rules.makeMove(next_move);
}

const Piece &Board::getPiece(const sf::Vector2f& p) const
{
  int x = p.x / config::figure_size;
  int y = p.y / config::figure_size;

  if (x < 0) x = 0; else if (x > 7) x = 7;
  if (y < 0) y = 0; else if (y > 7) y = 7;

  return m_rules.m_current_position[y * 8 + x];
}

void Board::takeFigure()
{
  // Filter useless events
  if (true == m_moving_figure)
    return ;

  // End of the game
  if (Status::Playing != m_rules.m_status)
    return ;

  // Mouse is out of bound of the chessboard
  if ((m_mouse.x < 0.0f) || (m_mouse.x > config::dimension.x))
    return ;
  if ((m_mouse.y < 0.0f) || (m_mouse.y > config::dimension.y))
    return ;

  for (uint8_t i = 0u; i < 32u; ++i)
    {
      if (m_figures[i].getGlobalBounds().contains(m_mouse.x, m_mouse.y))
        {
          const Piece& piece = getPiece(m_mouse);
          if (piece.color == m_rules.m_side)
            {
              m_piece = i;
              m_delta_pos = m_mouse - m_figures[i].getPosition();
              m_old_pos = m_figures[i].getPosition();
              m_moving_figure = true;
            }
        }
    }
}

void Board::releaseFigure()
{
  // Filter useless events
  if (false == m_moving_figure)
    return ;

  // End of the game
  if (Status::Playing != m_rules.m_status)
    return ;

  // Mouse is out of bound of the chessboard
  if ((m_mouse.x < 0.0f) || (m_mouse.x > config::dimension.x))
    return ;
  if ((m_mouse.y < 0.0f) || (m_mouse.y > config::dimension.y))
    return ;

  //
  sf::Vector2f p = m_figures[m_piece].getPosition()
                 +  sf::Vector2f(config::figure_size / 2, config::figure_size / 2);
  m_new_pos = sf::Vector2f(config::figure_size * int(p.x / config::figure_size),
                           config::figure_size * int(p.y / config::figure_size));

  if (m_old_pos == m_new_pos)
    {
      m_figures[m_piece].setPosition(m_new_pos);
      m_moving_figure = false;
      return ;
    }

  // Fast filter of illegal move.
  // Player is trying to release its picked figure on a figure of his side.
  const Piece& piece = getPiece(m_mouse);
  std::cout << piece << std::endl;
  if ((piece.color == m_rules.m_side) && (piece.type != PieceType::Empty))
    return ;

  // Create the note move
  std::string next_move(toChessNote(m_old_pos));
  next_move += toChessNote(m_new_pos);

  if (!m_rules.isValidMove(Move(next_move)))
    return ;

  noAnimation(next_move);
  //m_figures[m_piece].setPosition(m_new_pos);

  m_rules.makeMove(next_move);
  m_moving_figure = false;
  draw();
}

void Board::draw()
{
  // Draw the figure when the user is grabbing it
  if (m_moving_figure)
    {
      m_figures[m_piece].setPosition(m_mouse - m_delta_pos);
    }

  m_window.draw(m_sboard);

  for (uint8_t i = 0u; i < 32u; ++i)
    m_figures[i].move(config::border);

  for (uint8_t i = 0u; i < 32u; ++i)
    m_window.draw(m_figures[i]);

  m_window.draw(m_figures[m_piece]);

  for (uint8_t i = 0u; i < 32u; ++i)
    m_figures[i].move(-config::border);

  m_window.display();
}
