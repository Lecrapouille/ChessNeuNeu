#include "GUI/Board.hpp"
#include "GUI/Promotion.hpp"

static const uint8_t NoFigure = NbPieces + 1u;

Board::Board(Application& application, Rules &rules, Resources &resources)
  : GUI(application),
    m_resources(resources),
    m_rules(rules)
{
  ungrabFigure();
  m_mouse = sf::Vector2f(sf::Mouse::getPosition(window()));
  loadPosition(m_rules.m_board);
}

Board::~Board()
{
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

  // Grab a piece of its color
  Square sq = getSquare(mouse);
  if (Square::OOB == sq)
    return false;

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
  return true;
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
  if (m_updated)
    return ;

  //std::cout << "load board" << std::endl;
  //loadPosition(m_rules.m_board);

  m_updated = true;
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
          window().close();
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
              m_rules.applyMove(m_move);
              loadPosition(m_rules.m_board);
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
              window().close();
            }
          break;

        default:
          break;
        }
    }
}
