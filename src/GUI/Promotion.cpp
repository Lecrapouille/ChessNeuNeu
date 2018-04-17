#include "GUI/Promotion.hpp"

Promotion::Promotion(Application& application, Resources &resources, Piece &taken_piece, Color color)
  : GUI(application), m_resources(resources), m_taken_piece(taken_piece)
{
  // Load a dummy board with major pieces.

  m_board = Chessboard::Empty;
  m_board[sqC4] = { color, 0, 0, PieceType::Rook };
  m_board[sqD4] = { color, 0, 0, PieceType::Knight };
  m_board[sqE4] = { color, 0, 0, PieceType::Bishop };
  m_board[sqF4] = { color, 0, 0, PieceType::Queen };

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

const Piece &Promotion::getPiece(const sf::Vector2f& mouse) const
{
  int x = mouse.x / conf::dim::figure;
  int y = mouse.y / conf::dim::figure;

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
      if (m_resources.figures[i].getGlobalBounds().contains(m_mouse.x, m_mouse.y))
        {
          return getPiece(m_mouse);
        }
    }
  return NoPiece;
}

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
              m_taken_piece = taken_piece;
            }
          break;

        default:
          break;
        }
    }
}
