#include "Board.hpp"
#include <iostream>

Board::Board(States &states, sf::RenderWindow &window)
  : m_states(states),
    m_window(window),
    m_piece(0u),
    m_moving_figure(false)
{
  loadTextures();
  loadPosition();
}

Board::~Board()
{
}

void Board::loadTextures()
{
  bool res1 = m_textures[0].loadFromFile("../data/figures.png");
  bool res2 = m_textures[1].loadFromFile("../data/board.png");
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

void Board::loadPosition()
{
  int k = 0;

  for (uint8_t i = 0u; i < 8u; ++i)
    {
      for (uint8_t j = 0u; j < 8u; ++j)
        {
          int n = m_states.board[i][j];
          if (!n)
            continue;
          int x = abs(n) - 1;
          int y = (n > 0) ? 1 : 0;
          m_figures[k].setTextureRect(sf::IntRect(config::figure_size * x,
                                                  config::figure_size * y,
                                                  config::figure_size,
                                                  config::figure_size));
          m_figures[k].setPosition(config::figure_size * j,
                                   config::figure_size * i);
          ++k;
        }
    }

  for (uint32_t i = 0u; i < m_states.moves.length(); i += 5u)
    {
      move(m_states.moves.substr(i, 4u));
    }
}

void Board::move(const std::string& movement)
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

  // Castling if King did not move
  if (movement == "e1g1")
    {
      if (m_states.moves.find("e1") == std::string::npos)
        move("h1f1");
    }
  else if (movement == "e8g8")
    {
      if (m_states.moves.find("e8") == std::string::npos)
        move("h8f8");
    }
  else if (movement == "e1c1")
    {
      if (m_states.moves.find("e1") == std::string::npos)
        move("a1d1");
    }
  else if (movement == "e8c8")
    {
      if (m_states.moves.find("e8") == std::string::npos)
        move("a8d8");
    }
}

void Board::moveBack()
{
  // 5 is the string size for example "e2e4 "
  if (m_states.moves.length() >= 5)
    m_states.moves.erase(m_states.moves.length() - 5, 5);
  loadPosition();
}

//! \param move the new valid move like "e2e4".
//! \note: the move shall be valid !
void Board::animation(const std::string& movement)
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

  move(movement);
  m_states.moves += movement;
  m_states.moves += ' ';
  m_figures[m_piece].setPosition(m_new_pos);
}

void Board::takeFigure()
{
  // Filter useless events
  if (true == m_moving_figure)
    return ;

  for (uint8_t i = 0u; i < 32u; ++i)
    {
      if (m_figures[i].getGlobalBounds().contains(m_mouse.x, m_mouse.y))
        {
          m_piece = i;
          m_delta_pos = m_mouse - m_figures[i].getPosition();
          m_old_pos = m_figures[i].getPosition();
          m_moving_figure = true;
        }
    }
}

void Board::releaseFigure()
{
  // Filter useless events
  if (false == m_moving_figure)
    return ;

  m_moving_figure = false;
  sf::Vector2f p = m_figures[m_piece].getPosition()
                 +  sf::Vector2f(config::figure_size / 2, config::figure_size / 2);
  m_new_pos =  sf::Vector2f(config::figure_size * int(p.x / config::figure_size),
                            config::figure_size * int(p.y / config::figure_size));

  std::string movement(toChessNote(m_old_pos));
  movement += toChessNote(m_new_pos);
  move(movement);

  if (m_old_pos != m_new_pos)
    {
      m_states.moves += movement;
      m_states.moves += ' ';
    }

  m_figures[m_piece].setPosition(m_new_pos);
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
