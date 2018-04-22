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

#ifndef GUI_PROMOTION_HPP
#  define GUI_PROMOTION_HPP

#  include "Utils/GUI.hpp"
#  include "GUI/Dimension.hpp"
#  include "Chess/Rules.hpp"

// ***********************************************************************************************
//! \brief
// ***********************************************************************************************
class Promotion: public GUI
{
public:

  //! \brief Constructor get references on game rules
  //! and the main window needed for drawing the GUI.
  Promotion(Application& application, Piece &taken_piece, Color color);

  //! \brief Destructor. Release only GUI resources
  //! but not game rules.
  ~Promotion();

  //! \brief Action on mouse button pressed event.
  //! Grab the piece (if present).
  Piece takeFigure();

  //! \brief Save the current mouse position and remove
  //! the board border dimension.
  inline void mousePosition(const sf::Vector2i p)
  {
    m_mouse = sf::Vector2f(p) - config::dim::border;
  }

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

  //! \brief Memorize the last moving figure.
  Piece             &m_taken_piece;
  //! \brief
  chessboard         m_board;
  //! \brief Textures of the chess board and figures.
  sf::Texture        m_textures[2];
  //! \brief Chess pieces are a textured square knowing its position.
  sf::Sprite         m_figures[NbPieces];
  //! \brief Chessboard is a textured square knowing its position.
  sf::Sprite         m_sboard;
  //! \brief Memorize the mouse position when moving a figure.
  sf::Vector2f       m_mouse;
};

#endif
