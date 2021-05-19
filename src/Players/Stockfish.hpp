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

#ifndef STOCKFISH_HPP
#  define STOCKFISH_HPP

#  include "Player.hpp"
#  include "Utils/IPC.hpp"

// *****************************************************************************
//! \brief Implement a chess player. Call the stockfish program and
//! communicate with it through a bidirectional pipe.
// *****************************************************************************
class Stockfish: public IPC, public IPlayer
{
public:

    //! \brief Constructor.
    //! \param fen if the game shall starts from a loaded chessboard: in this
    //! case use a Forsyth-Edwards Notation string. In the case you desire
    //! starting from the initial chessboard position pass an empty string.
    Stockfish(const Rules &rules, const Color side, std::string const& fen = "");
    //! \brief return the Stockfish move.
    virtual std::string play() override;
    virtual void abort() override;

private:

    //! \brief start the game from a loaded chessboard. Use Forsyth-Edwards
    //! Notation.
    std::string m_initial_board;
    //! \brief We need to access to the chess rules for getting the list of
    //! played moves.
    const Rules &m_rules;
    //! \brief For leaving loop
    bool m_aborting = false;
};

#endif
