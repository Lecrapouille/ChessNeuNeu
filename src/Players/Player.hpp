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

#ifndef PLAYER_HPP
#  define PLAYER_HPP

#  include "Chess/Rules.hpp"

// ***********************************************************************************************
//! \brief Define here
// ***********************************************************************************************
enum PlayerType { HumanPlayer, StockfishIA, TscpIA, NeuNeuIA };

// ***********************************************************************************************
//! \brief Interface class for a chess player.
// ***********************************************************************************************
class IPlayer
{
public:

  IPlayer(const PlayerType type, const Color side)
    : m_type(type), m_side(side)
  {
  }

  virtual ~IPlayer()
  {
  }

  //! \brief Compute and return a legal move (like "e7e8q") or
  //! IPlayer::none for stalemate case or IPlayer::error in case
  //! of internal error. This method can take long minutes to be
  //! done. This is normal because compuatations can be heavy.
  //! Therefore this method should be called from a thread.
  virtual std::string play() = 0;

  //! \bref Abort signal for halting properlu play()
  virtual void abort() = 0;

  inline Color side() const
  {
    return m_side;
  }

  inline PlayerType type() const
  {
    return m_type;
  }

  static constexpr const char* error = "error";
  static constexpr const char* none = "none";

private:

  PlayerType m_type;
  Color m_side;
};

std::ostream& operator<<(std::ostream& os, const PlayerType& p);
const char *playerName(const PlayerType p);
PlayerType playerType(const std::string& player);

#endif
