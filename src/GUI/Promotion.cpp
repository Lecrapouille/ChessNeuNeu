#include "GUI/Promotion.hpp"

Promotion::Promotion(Application& application, Piece &taken_piece, Color color)
  : GUI(application), m_taken_piece(taken_piece)
{
  m_board = Chessboard::Empty;
  m_board[sqC4] = { color, 0, 0, 0, PieceType::Rook };
  m_board[sqD4] = { color, 0, 0, 0, PieceType::Knight };
  m_board[sqE4] = { color, 0, 0, 0, PieceType::Bishop };
  m_board[sqF4] = { color, 0, 0, 0, PieceType::Queen };

  loadTextures();
  loadPosition();
}

Promotion::~Promotion()
{
}

void Promotion::loadTextures()
{
  bool res1 = m_textures[0].loadFromFile("data/figures.png");
  bool res2 = m_textures[1].loadFromFile("data/board.png");
  if ((false == res1) || (false == res2))
    {
      std::cerr << "Failed loading textures" << std::endl;
    }

  for (uint8_t i = 0u; i < NbPieces; ++i)
    {
      m_figures[i].setTexture(m_textures[0]);
    }
  m_sboard.setTexture(m_textures[1]);
}

void Promotion::loadPosition()
{
  uint8_t k = 0;

  for (uint8_t ij = sqC4; ij <= sqF4; ++ij)
    {
      const Piece p = m_board[ij];
      if (p.type == PieceType::Empty)
        continue;

      const uint32_t x = p.type - 1u;
      const uint32_t y = p.color;
      m_figures[k].setTextureRect(sf::IntRect(config::dim::figure * x,
                                              config::dim::figure * y,
                                              config::dim::figure,
                                              config::dim::figure));
      m_figures[k].setPosition(config::dim::figure * COL(ij),
                               config::dim::figure * ROW(ij));
      ++k;
    }

  // Hide unused pieces
  for (; k < NbPieces; ++k)
    m_figures[k].setPosition(-1000, -1000);
}

const Piece &Promotion::getPiece(const sf::Vector2f& mouse) const
{
  int x = mouse.x / config::dim::figure;
  int y = mouse.y / config::dim::figure;

  // Paranoia: SFML allow click event with mouse position
  // outside the chessboard.
  if (x < 0) x = 0; else if (x > 7) x = 7;
  if (y < 0) y = 0; else if (y > 7) y = 7;

  return m_board[y * 8 + x];
}

Piece Promotion::takeFigure()
{
  // Find which piece is in the mouse cursor
  for (uint8_t i = 0u; i < NbPieces; ++i)
    {
      if (m_figures[i].getGlobalBounds().contains(m_mouse.x, m_mouse.y))
        {
          return getPiece(m_mouse);
        }
    }
  return NoPiece;
}

void Promotion::draw(const float /*dt*/)
{
  window().draw(m_sboard);

  for (uint8_t i = 0u; i < NbPieces; ++i)
    m_figures[i].move(config::dim::border);

  for (uint8_t i = 0u; i < NbPieces; ++i)
    window().draw(m_figures[i]);

  //window().draw(m_figures[m_taken_piece]);

  for (uint8_t i = 0u; i < NbPieces; ++i)
    m_figures[i].move(-config::dim::border);

  window().display();
}

void Promotion::update(const float /*dt*/)
{
}

bool Promotion::running()
{
  return window().isOpen() &&
    (PieceType::Empty == m_taken_piece.type);
}

void Promotion::handleInput()
{
  sf::Event event;
  Piece taken_piece = NoPiece;

  mousePosition(sf::Mouse::getPosition(window()));
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
              m_taken_piece = taken_piece;
            }
          break;

        default:
          break;
        }
    }
}
