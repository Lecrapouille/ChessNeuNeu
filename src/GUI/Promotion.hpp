#ifndef GUI_PROMOTION_HPP
#  define GUI_PROMOTION_HPP

#  include "Utils/GUI.hpp"
#  include "GUI/Resources.hpp"
#  include "Chess/Rules.hpp"

// ***********************************************************************************************
//! \brief GUI managing the pawn promotion. Display a board with possible pieces to promote.
//! When the player selects one piece, this piece is used for the game.
// ***********************************************************************************************
class Promotion: public GUI
{
public:

  //! \brief Constructor get references on game rules
  //! and the main window needed for drawing the GUI.
  Promotion(Application& application, Resources &resources, Piece &taken_piece, Color color);

  //! \brief Destructor. Release only GUI resources
  //! but not game rules.
  ~Promotion();

  //! \brief Action on mouse button pressed event.
  //! Grab the piece (if present).
  Piece takeFigure();

  const Piece& getPiece(const sf::Vector2f& p) const;

private:

  //! \brief Load chessboard and pieces textures.
  void loadTextures();

  //! \brief Place pieces on their position.
  void loadPosition();

  //! \brief Draw the chessboard and pieces.
  virtual void draw(const float dt) override;
  virtual void update(const float dt) override;
  virtual void handleInput() override;
  virtual bool running() override;

private:

  //! \brief Reference on loaded resources (textures ...)
  Resources         &m_resources;
  //! \brief Memorize the last moving figure.
  Piece             &m_taken_piece;
  //! \brief
  chessboard         m_board;
  //! \brief Memorize the mouse position when moving a figure.
  sf::Vector2f       m_mouse;
};

#endif
