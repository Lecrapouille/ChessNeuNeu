//=====================================================================
// ChessNeuNeu: Non serious chess engine for learning neural networks.
// Copyright 2018 -- 2022 Quentin Quadrat <lecrapouille@gmail.com>
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

// *****************************************************************************
//! \brief Define here all type of chess players. Currently implemented:
//! -- Human player: Use the GUI and a mouse for moving pieces.
//! -- Stockfish: play against Stockfish software (you shall install it)
//! -- TSCP: play against TSCP software (you shall install it)
//! -- Loki: play against Loki3 software (you shall install it)
//! -- NeuNeu: play against my neural network IA.
// *****************************************************************************
enum PlayerType { HumanPlayer, StockfishIA, TscpIA, LokiIA, NeuNeuIA };

// *****************************************************************************
//! \brief Abstract class for a chess player. If you desire to add your own IA
//! makes it inherites from this class and implement the play() method which
//! shall return a valid move as a string.
// *****************************************************************************
class IPlayer
{
public:

    //! \brief Constructor.
    //! \param type of player you desire (human, external software, IA ...)
    //! \param side: the color to play (white or black)
    IPlayer(const PlayerType type, const Color side)
        : m_type(type), m_side(side)
    {}

    //! \brief Need because of virtual methods.
    virtual ~IPlayer() = default;

    //! \brief Compute and return a legal move (like "e7e8q") or return
    //! Move::none for stalemate case or return Move::error in case of internal
    //! error.
    //!
    //! This method can take long minutes to be done. This is normal because
    //! computations can be heavy. Therefore this method should be called from a
    //! thread.
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

    //! \brief Used by the play() method when an internal error has occured.
    static constexpr const char* error = "::error";

    //! \brief Used by the play() method when the user want to quit ChessNeuNeu.
    static constexpr const char* quitting = "::quitting";

private:

    PlayerType m_type;
    Color m_side;
};

//! \brief Print on console the player type.
std::ostream& operator<<(std::ostream& os, const PlayerType& p);
//! \brief Return the player type as string from its enum.
const char *playerType(const PlayerType p);
//! \brief Return the player enum from a string.
PlayerType playerType(const std::string& player);

#endif
