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

#ifndef GUI_BOARD_HPP
#  define GUI_BOARD_HPP

#  include "Utils/GUI.hpp"
#  include "Players/Human.hpp"
#  include "GUI/Resources.hpp"
#  include "Chess/Rules.hpp"
#  include <thread>
#  include <atomic>

// ***********************************************************************************************
//! \brief Class displaying a Chess board, its figures and let to the player moving
//! pieces. Inspired by the Youtube video 'Let's make 16 games in C++: Chess' by FamTrinli.
// ***********************************************************************************************
class Board: public GUI
{
public:

  //! \brief Constructor.
  Board(Application& application, Rules &rules, Resources &resources, IPlayer **players);

  //! \brief Destructor
  ~Board();

private:

  void play();

  //! \brief Place figures on their position.
  void loadPosition(chessboard const& board);

  //! \brief Action on mouse button pressed event.
  //! Grab the piece (if present).
  bool takeFigure(sf::Vector2f const& mouse);

  //! \brief Action on mouse button pressed event.
  //! Grab the piece (if present).
  bool releaseFigure(sf::Vector2f const& mouse);

  //! \brief Inherit from GUI class. Draw the chessboard and pieces.
  virtual void draw(const float dt) override;

  //! \brief Inherit from GUI class. Update GUI.
  virtual void update(const float dt) override;

  //! \brief Inherit from GUI class. Manage mouse and keyboard events.
  virtual void handleInput() override;

  //! \brief Inherit from GUI class. Return if GUI is alive.
  virtual bool running() override;

  void ungrabFigure();
  bool grabbedFigure() const;

  //! \brief Get the Piece from a position
  Piece const& getPiece(sf::Vector2f const& mouse) const;

  //! \brief Get the chessboard square from a position
  Square getSquare(sf::Vector2f const& mouse) const;

  void animate(const std::string& move);
  sf::Vector2f toCoord(const char a, const char b) const;

private:

  //! \brief Reference on loaded resources (textures ...)
  Resources         &m_resources;
  //! \brief Reference on the game rules.
  Rules             &m_rules;
  //! \brief Memorize the taken figure by the user.
  //! Refer to m_resources.figures[x].
  uint8_t           m_grabbed;
  //! \brief the start square of a piece movement
  uint8_t           m_from = Square::OOB;
  //! \brief the arrival square of a piece movement
  uint8_t           m_to = Square::OOB;
  //! \brief Memorize the mouse position when moving a figure.
  sf::Vector2f       m_mouse;
  bool               m_updated = true;
  std::string        m_move;

  IPlayer**          m_players;
  std::string        m_opponent_move;
  std::atomic_bool   m_running_thread{true};
  std::atomic_bool   m_animating{false};
  std::thread        m_thread;
  using MuxGuard = std::lock_guard<std::mutex>;
  mutable std::mutex m_lock;
};

#endif
