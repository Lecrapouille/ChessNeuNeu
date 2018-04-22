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

#ifndef GUI_DIMENSION_HPP
#  define GUI_DIMENSION_HPP

#  include <SFML/Graphics.hpp>

// ***********************************************************************************************
//! \brief Define constants for the board
// ***********************************************************************************************
namespace config
{
  namespace dim
  {
    //! \brief the dimension of the squared texture for each piece.
    const int          figure = 56;
    //! \brief The boder dimension (x, y) of the chessboard.
    const sf::Vector2f border{28, 28};
    //! \brief Chessboard dimension
    const sf::Vector2f board
    {
      8.0f * 2.0f * config::dim::border.x,
      8.0f * 2.0f * config::dim::border.y
    };
  }
};

#endif
