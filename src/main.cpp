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

#include "main.hpp"
#include "GUI/Board.hpp"
#include "Players/Stockfish.hpp"
#include "Players/TSCP.hpp"
#include "Players/Loki.hpp"
#include "Players/NeuNeu.hpp"
#include "Players/Human.hpp"

// -----------------------------------------------------------------------------
void ChessNeuNeu::createPlayer(const PlayerType type, const Color side)
{
    switch (type)
    {
    case PlayerType::StockfishIA:
        m_players[side] = std::make_shared<Stockfish>(m_rules, side, m_fen);
        break;
    case PlayerType::TscpIA:
        m_players[side] = std::make_shared<Tscp>(m_rules, side);
        break;
    case PlayerType::LokiIA:
        m_players[side] = std::make_shared<Loki>(m_rules, side);
        break;
    case PlayerType::NeuNeuIA:
        m_players[side] = std::make_shared<NeuNeu>(m_rules, side);
        break;
    case PlayerType::HumanPlayer:
        m_players[side] = std::make_shared<Human>(m_rules, side);
        break;
    default:
        throw std::string("createPlayer: Unknown PlayerType");
        break;
    }
}

// -----------------------------------------------------------------------------
ChessNeuNeu::ChessNeuNeu(const PlayerType white, const PlayerType black, std::string const& fen)
    : m_resources("figures.png", "board.png"), m_fen(fen), m_rules(fen)
{
    init(white, black);
}

// -----------------------------------------------------------------------------
ChessNeuNeu::ChessNeuNeu(const PlayerType white, const PlayerType black)
    : m_resources("figures.png", "board.png")
{
    init(white, black);
}

// -----------------------------------------------------------------------------
void ChessNeuNeu::init(const PlayerType white, const PlayerType black)
{
    createPlayer(white, Color::White);
    createPlayer(black, Color::Black);

    // Be sure to play with Kings (chessboard with no Kings is only
    // used for Neural trainings and unit tests).
    assert(false == m_rules.m_no_kings);

    // Debug
    std::cout
            << m_players[Color::White]->side()
            << " color is played by: "
            << m_players[Color::White]->type()
            << std::endl
            << m_players[Color::Black]->side()
            << " color is played by: "
            << m_players[Color::Black]->type()
            << std::endl << std::endl
            << m_rules.m_board << std::endl
            << m_rules.m_side << " are thinking ... "
            << std::flush;

    // Create GUIs
    m_gui_board = std::make_unique<Board>(*this, m_rules, m_resources, m_players);
}

// -----------------------------------------------------------------------------
//! \brief Parser the command-line option
// -----------------------------------------------------------------------------
static std::string getCmdOption(int argc, char* argv[], const std::string& short_option,
                                const std::string& long_option)
{
    for (int i = 0; i < argc; ++i)
    {
        std::string arg(argv[i]);
        if ((arg == short_option) || (arg == long_option))
        {
            if (i+1 < argc)
                return argv[i+1];
            return argv[i];
        }
    }
    return {};
}

// -----------------------------------------------------------------------------
int main(int argc, char** argv)
{
    // Initialize random seed
    srand(time(NULL));

    // Help/Usage
    if (getCmdOption(argc, argv, "-h", "--help") != "")
    {
        std::cout << "Usage:\n  " << argv[0] << " --white NAME --black NAME [--fen FEN]\n"
                  << "With:\n  NAME: human | stockfish | loki | tcsp | neuneu\n"
                  << "  FEN: Optional oard position in Forsyth-Edwards notation. See https://lichess.org/editor\n";
        return EXIT_SUCCESS;
    }

    // Parse the command-line options
    std::string w(getCmdOption(argc, argv, "-w", "--white"));
    std::string b(getCmdOption(argc, argv, "-b", "--black"));
    std::string fen(getCmdOption(argc, argv, "-f", "--fen"));

    try
    {
        std::unique_ptr<ChessNeuNeu> chess;

        // Get Player types from command-line options --white and --black.
        // An exception is thrown if player type is badly typed.
        PlayerType Whites = playerType(w != "" ? w : "human");
        PlayerType Blacks = playerType(b != "" ? b : "neuneu");

        // Optional: start the game with a given chessboard with the
        // comand-line --fen (Forsyth-Edwards notation).
        if (fen.empty())
        {
            chess = std::make_unique<ChessNeuNeu>(Whites, Blacks);
        }
        else
        {
            chess = std::make_unique<ChessNeuNeu>(Whites, Blacks, fen);
        }

        // Launch the GUI thread which will also start the game logic thread
        chess->loop(chess->gui());
    }
    catch (std::string const& msg)
    {
        std::cerr << "Fatal: " << msg << std::endl;
        return EXIT_FAILURE;
    }
    catch (std::exception const& e)
    {
        std::cerr << "Fatal: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
