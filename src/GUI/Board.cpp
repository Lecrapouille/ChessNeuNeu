#include "GUI/Board.hpp"

Board::Board(Application& application, Rules& rules, IPlayer** players)
  : GUI(application),
    m_rules(rules),
    m_taken_piece(0u),
    m_moving_figure(false),
    m_players(players)
{
  loadTextures();
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

void Board::loadTextures()
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

void Board::loadPosition(const chessboard& board)
{
  uint8_t k = 0;

  for (uint8_t ij = 0u; ij < NbSquares; ++ij)
    {
      const Piece p = board[ij];
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

//! \param move the newly played move in format like "e2e4".
//! \note: the move shall be valid !
void Board::moveWithAnimation(const std::string& move)
{
  m_old_pos = toCoord(move[0], move[1]);
  m_new_pos = toCoord(move[2], move[3]);

  uint8_t taken_piece = NbPieces;
  for (uint8_t i = 0u; i < NbPieces; ++i)
    {
      if (m_figures[i].getPosition() == m_old_pos)
        {
          taken_piece = i;
        }
    }

  // Assert piece taken (else that would mean that
  // given move was illegal)
  assert(taken_piece != NbPieces && "Illegal move");
  m_taken_piece = taken_piece;

  // Smooth animation
  const uint32_t smooth = 50;
  for (uint32_t k = 0; k < smooth; ++k)
    {
      sf::Vector2f p = m_new_pos - m_old_pos;
      m_figures[m_taken_piece].move(p.x / smooth,
                                    p.y / smooth);
      draw(0.0f);
    }

  moveWithoutAnimation(move);
}

void Board::moveWithoutAnimation(const std::string& move)
{
  sf::Vector2f oldPos = toCoord(move[0], move[1]);
  sf::Vector2f newPos = toCoord(move[2], move[3]);

  for (uint8_t i = 0u; i < NbPieces; ++i)
    {
      if (m_figures[i].getPosition() == newPos)
        {
          m_figures[i].setPosition(-100, -100);
        }
    }

  for (uint8_t i = 0u; i < NbPieces; ++i)
    {
      if (m_figures[i].getPosition() == oldPos)
        {
          m_figures[i].setPosition(newPos);
        }
    }

  m_figures[m_taken_piece].setPosition(m_new_pos);

  if (move == "e1g1")
    {
      if (m_rules.m_moved.find("e1") == std::string::npos)
        moveWithoutAnimation("h1f1");
    }
  else if (move == "e8g8")
    {
      if (m_rules.m_moved.find("e8") == std::string::npos)
        moveWithoutAnimation("h8f8");
    }
  else if (move == "e1c1")
    {
      if (m_rules.m_moved.find("e1") == std::string::npos)
        moveWithoutAnimation("a1d1");
    }
 else if (move == "e8c8")
    {
      if (m_rules.m_moved.find("e8") == std::string::npos)
        moveWithoutAnimation("a8d8");
    }
}

const Piece &Board::getPiece(const sf::Vector2f& mouse) const
{
  int x = mouse.x / config::dim::figure;
  int y = mouse.y / config::dim::figure;

  // Paranoia: SFML allow click event with mouse position
  // outside the chessboard.
  if (x < 0) x = 0; else if (x > 7) x = 7;
  if (y < 0) y = 0; else if (y > 7) y = 7;

  return m_rules.m_board[y * 8 + x];
}

void Board::takeFigure()
{
  // Filter useless events
  if (true == m_moving_figure)
    return ;

  // Do not touch opponent figures
  if (PlayerType::HumanPlayer != m_players[m_rules.m_side]->type())
    return ;

  // No end game (checkmate, stalemate ...)
  if (Status::Playing != m_rules.m_status)
    return ;

  // Mouse is out of bound of the chessboard
  if ((m_mouse.x < 0.0f) || (m_mouse.x > config::dim::board.x) ||
      (m_mouse.y < 0.0f) || (m_mouse.y > config::dim::board.y))
    return ;

  // Find which piece is in the mouse cursor
  for (uint8_t i = 0u; i < NbPieces; ++i)
    {
      if (m_figures[i].getGlobalBounds().contains(m_mouse.x, m_mouse.y))
        {
          const Piece& piece = getPiece(m_mouse);
          if (piece.color == m_rules.m_side)
            {
              m_taken_piece = i;
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

  // Do not touch opponent figures
  if (PlayerType::HumanPlayer != m_players[m_rules.m_side]->type())
    return ;

  // Mouse is out of bound of the chessboard
  if ((m_mouse.x < 0.0f) || (m_mouse.x > config::dim::board.x) ||
      (m_mouse.y < 0.0f) || (m_mouse.y > config::dim::board.y))
    return ;

  //
  sf::Vector2f p = m_figures[m_taken_piece].getPosition()
                 +  sf::Vector2f(config::dim::figure / 2, config::dim::figure / 2);
  m_new_pos = sf::Vector2f(config::dim::figure * int(p.x / config::dim::figure),
                           config::dim::figure * int(p.y / config::dim::figure));

  if (m_old_pos == m_new_pos)
    {
      m_figures[m_taken_piece].setPosition(m_new_pos);
      m_moving_figure = false;
      return ;
    }

  // Fast filter of illegal move.
  // Player is trying to release its picked figure on a figure of his side.
  const Piece& piece = getPiece(m_mouse);
  if (piece.color == m_rules.m_side)
    return ;

  // Create the note move
  std::string next_move(toChessNote(m_old_pos));
  next_move += toChessNote(m_new_pos);

  // Pawn promotion
  const Piece& piece2 = getPiece(m_old_pos);
#if 0
  if ((piece2.type == PieceType::Pawn) /*&& ()*/)
    {
      Piece promote = NoPiece;
      m_application.loop(new Promotion(m_application, promote, m_rules.m_side));
      next_move += piece2char(promote);
    }
#endif
  // Accept or refuse the move
  if (!m_rules.isValidMove(next_move))
    return ;

  moveWithoutAnimation(next_move);
  reinterpret_cast<Human*>(m_players[m_rules.m_side])->notified(next_move);
  m_moving_figure = false;
}

void Board::draw(const float /*dt*/)
{
  // Draw the figure when the user is grabbing it
  if (m_moving_figure)
    {
      m_figures[m_taken_piece].setPosition(m_mouse - m_delta_pos);
    }

  window().draw(m_sboard);

  for (uint8_t i = 0u; i < NbPieces; ++i)
    m_figures[i].move(config::dim::border);

  for (uint8_t i = 0u; i < NbPieces; ++i)
    window().draw(m_figures[i]);

  window().draw(m_figures[m_taken_piece]);

  for (uint8_t i = 0u; i < NbPieces; ++i)
    m_figures[i].move(-config::dim::border);

  window().display();
}

void Board::update(const float /*dt*/)
{
  std::string move;
  {
    MuxGuard g(m_lock);
    if (m_opponent_move.empty())
      return ;
    move = m_opponent_move;
    m_opponent_move.clear();
  }
  m_animating = true;
  moveWithAnimation(move);
  m_animating = false;
}

bool Board::running()
{
  //m_running_thread = window().isOpen();
  return m_running_thread;
}

void Board::handleInput()
{
  sf::Event event;

  mousePosition(sf::Mouse::getPosition(window()));

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
          //FIXME temporaire
          //m_application.loop(new Promotion(m_application, p,
          //                                 m_rules.m_side));
          //std::cout << "Board::Promotion: " << p << std::endl;

          takeFigure();
          break;

        case sf::Event::MouseButtonReleased:
          releaseFigure();
          break;

        case sf::Event::KeyPressed:
          if (event.key.code == sf::Keyboard::BackSpace)
            {
              m_rules.revertLastMove();
              loadPosition(m_rules.m_board);
            }
          break;

        default:
          break;
        }
    }
}
