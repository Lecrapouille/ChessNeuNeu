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
#  include <memory>

// *****************************************************************************
//! \brief Main window showing the chessboard and allowing two opponents (human
//! or engine or IA) to play chess on this board and arbitrated by a chess rules
//! class.
// *****************************************************************************
class ChessNeuNeu : public Application
{
public:

    //! \brief Constructor. Start with initial board and white to play.
    ChessNeuNeu(const PlayerType Whites, const PlayerType Blacks);

    //! \brief Constructor. Start with a given board using the Forsyth-Edwards
    //! notation.
    //! \param fen: the board using the Forsyth-Edwards notation.
    //! You can use this site https://lichess.org/editor for generating FEN strings.
    ChessNeuNeu(const PlayerType Whites, const PlayerType Blacks, std::string const& fen);

    //! \brief Return the main GUI (board)
    GUI& gui() { return *m_gui_board; }

private:

    //! \brief Shall only be called by constructors. This method is here just
    //! for factorizing the code.
    void init(const PlayerType white, const PlayerType black);

    //! \brief Factory Create a player: human, IA with the desired color.
    void createPlayer(const PlayerType type, const Color side);

public:

    // SFML Textures, sprites ...
    Resources m_resources;

    //! \brief Save the chessboard position passed in command-line option. This
    //! will be used for Stockfish.
    std::string m_fen;

    //! \the Chess referee and game states.
    Rules m_rules;

    //! \brief The two opponents.
    std::shared_ptr<IPlayer> m_players[2];

    //! \brief The GUI board.
    std::unique_ptr<GUI> m_gui_board;
};

#endif
