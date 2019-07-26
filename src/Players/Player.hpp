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
//! \brief Define here all type of chess players. Currently implemented:
//! -- HumanPlayer: Use the GUI and a mouse for moving pieces
//! -- StockfishIA: play against Stockfish software (you shall install it)
//! -- TscpIA: play against TSCP software (you shall install it)
//! -- NeuNeuIA: play against my neural network IA.
// ***********************************************************************************************
enum PlayerType { HumanPlayer, StockfishIA, TscpIA, NeuNeuIA };

// ***********************************************************************************************
//! \brief Abstract class for a chess player. If you desire to add your own IA makes it inherites
//! from this class and implement the play() method which shall return a valid move as a string.
// ***********************************************************************************************
class IPlayer
{
public:

  //! \brief Constructor.
  //! \param type of player you desire (human, external software, IA ...)
  //! \param side: the color to play (white or black)
  IPlayer(PlayerType const type, Color const side)
    : m_type(type), m_side(side)
  {
  }

  //! \brief Destructor. Nothing made.
  virtual ~IPlayer()
  {
  }

  //! \brief Compute and return a legal move (like "e7e8q") or return
  //! IPlayer::none for stalemate case or return IPlayer::error in
  //! case of internal error.
  //!
  //! This method can take long minutes to be done. This is normal
  //! because computations can be heavy. Therefore this method should
  //! be called from a thread.
  virtual std::string play() = 0;

  //! \brief Abort signal for halting properly the play() method.
  //! Implement it as you desired (usually a simple bool).
  virtual void abort() = 0;

  //! \brief Getter returning the color of the play (white/black).
  inline Color side() const
  {
    return m_side;
  }

  //! \brief Getter returning the color of the type of player.
  inline PlayerType type() const
  {
    return m_type;
  }

  //! \brief Used by the play() method when an internal error has
  //! occured.
  static constexpr const char* const error = "error";

  //! \brief Used by the play() method when no move are available
  //! (like stalemate).
  static constexpr const char* const none = "none";

private:

  PlayerType m_type;
  Color m_side;
};

//! \brief Print on console the player type.
std::ostream& operator<<(std::ostream& os, PlayerType const& p);
//! \brief Return the player type as string from its enum.
const char* playerType(PlayerType const p);
//! \brief Return the player enum from a string.
PlayerType playerType(std::string const& player);

#endif
