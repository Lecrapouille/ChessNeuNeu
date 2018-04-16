#ifndef GUI_BOARD_HPP
#  define GUI_BOARD_HPP

#  include "Utils/GUI.hpp"
#  include "GUI/Dimension.hpp"
#  include "Players/Human.hpp"
#  include <thread>
#  include <atomic>

// ***********************************************************************************************
//! \brief Class displaying a Chess board, its figures and let to the player moving
//! pieces. Inspired by the Youtube video 'Let's make 16 games in C++: Chess' by FamTrinli.
// ***********************************************************************************************
class Board: public GUI
{
public:

  //! \brief Constructor get references on game rules
  //! and the main window needed for drawing the GUI.
  Board(Application&, Rules&, IPlayer* players[2]);

  //! \brief Destructor. Release only GUI resources
  //! but not game rules.
  ~Board();

private:

  void play();

  //! \brief Draw the chessboard and pieces.
  virtual void draw(const float dt) override;
  virtual void update(const float dt) override;
  virtual void handleInput() override;
  virtual bool running() override;

  //! \brief Move a piece with smooth displacement on the chessboard.
  void moveWithAnimation(const std::string& move);

  //! \brief Action on mouse button pressed event.
  //! Grab the piece (if present).
  void takeFigure();

  //! \brief Action on mouse button pressed event.
  //! Grab the piece (if present).
  void releaseFigure();

  //! \brief Save the current mouse position and remove
  //! the board border dimension.
  inline void mousePosition(const sf::Vector2i p)
  {
    m_mouse = sf::Vector2f(p) - config::dim::border;
  }

  const Piece& getPiece(const sf::Vector2f& p) const;

  //! \brief Place pieces on their position.
  void loadPosition(const chessboard& board);

  //! \brief Load chessboard and pieces textures.
  void loadTextures();

  //! \brief Move a piece with direct displacement on the chessboard.
  void moveWithoutAnimation(const std::string& move);

  //! \brief Convert a figure position into chess notation.
  //! \param p the position of the piece.
  //! \return the chess move (ie "e2e4").
  inline std::string toChessNote(const sf::Vector2f& p) const
  {
    char s[3] =
      {
        char(p.x / config::dim::figure + 'a'),
        char('8' - p.y / config::dim::figure),
        '\0'
      };
    return s;
  }

  //! \brief Convert a chess movement into a 2D position on the
  //! chessboard.
  inline sf::Vector2f toCoord(const char a, const char b) const
  {
    int x = int(a) - 'a';
    int y = '8' - int(b);
    return sf::Vector2f(x * config::dim::figure,
                        y * config::dim::figure);
  }

//private:

  //! \brief Reference on the game rules.
  Rules             &m_rules;
  //! \brief Textures of the chess board and figures.
  sf::Texture        m_textures[2];
  //! \brief Chess pieces are a textured square knowing its position.
  sf::Sprite         m_figures[NbPieces];
  //! \brief Chessboard is a textured square knowing its position.
  sf::Sprite         m_sboard;
  //! \brief Memorize the last moving figure.
  uint32_t           m_taken_piece;
  //! \brief Memorize the mouse position when moving a figure.
  sf::Vector2f       m_mouse;
  //! \brief Flag when mouse events: mouse pressed, released.
  bool               m_moving_figure;
  //! \brief Memorize the old position of a figure.
  sf::Vector2f       m_old_pos;
  //! \brief Memorize the new position of a figure.
  sf::Vector2f       m_new_pos;
  //! \brief delta position when the user is moving a piece.
  sf::Vector2f       m_delta_pos;

  IPlayer** m_players;
  std::atomic_bool   m_running_thread{true};
  std::thread        m_thread;

  using MuxGuard = std::lock_guard<std::mutex>;
  mutable std::mutex m_lock;
  std::atomic_bool  m_animating{false};
  std::string m_opponent_move;
};

#endif
