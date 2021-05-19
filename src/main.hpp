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

#ifndef MAIN_HPP
#  define MAIN_HPP

#  include "GUI/Resources.hpp"
#  include "Utils/GUI.hpp"
#  include "Players/Player.hpp"


// ***********************************************************************************************
//! \brief Main window showing the chessboard and allowing two players to play chess on this board.
// ***********************************************************************************************
class ChessNeuNeu : public Application
{
public:

    //! \brief Constructor. Start with initial board and white to play.
    ChessNeuNeu(const PlayerType Whites, const PlayerType Blacks);

    //! \brief Constructor. Start with a given board using the Forsyth-Edwards notation.
    ChessNeuNeu(const PlayerType Whites, const PlayerType Blacks, std::string const& fen);

    //! \brief Create the GUI and and the start a t
    void run();

private:

    //! \brief Shall only be called by constructors. This method is here just
    //! for factorizing the code.
    void init(const PlayerType white, const PlayerType black);

    //! \brief Factory Create a player: human, IA with the desired color.
    IPlayer *createPlayer(const PlayerType type, const Color side);

public:

    // Textures ...
    Resources m_resources;

    //! \brief Save the chessboard passed in command-line option.
    //! This will be used for Stockfish.
    std::string m_fen;

    //! \the Chess referee and game states.
    Rules    rules;

    //! \brief The
    //FIXME std::unique_ptr<IPlayer> m_players[2];
    IPlayer* players[2];
};

#endif
