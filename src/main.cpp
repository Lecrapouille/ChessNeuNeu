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

#include "main.hpp"
#include "GUI/Board.hpp"
#include "Players/Stockfish.hpp"
#include "Players/TSCP.hpp"
#include "Players/NeuNeu.hpp"
#include "Players/Human.hpp"
#include "CmdParser/cmdparser.hpp"

// ***********************************************************************************************
//! \brief
// ***********************************************************************************************
IPlayer *ChessNeuNeu::createPlayer(const PlayerType type, const Color side)
{
  switch (type)
    {
    case PlayerType::StockfishIA:
      return new Stockfish(rules, side, m_fen);
    case PlayerType::TscpIA:
      return new Tscp(rules, side);
    case PlayerType::NeuNeuIA:
      return new NeuNeu(rules, side);
    case PlayerType::HumanPlayer:
      return new Human(rules, side);
    default:
      throw std::string("createPlayer: Unknown PlayerType");
      break;
    }
}

// ***********************************************************************************************
//! \brief \param fen: the board using the Forsyth-Edwards notation. You can use this site
//! https://lichess.org/editor for generating FEN strings.
// ***********************************************************************************************
ChessNeuNeu::ChessNeuNeu(const PlayerType white, const PlayerType black, std::string const& fen)
  : m_fen(fen), rules(fen)
{
  init(white, black);
}

// ***********************************************************************************************
//! \brief
// ***********************************************************************************************
ChessNeuNeu::ChessNeuNeu(const PlayerType white, const PlayerType black)
{
  init(white, black);
}

// ***********************************************************************************************
//! \brief
// ***********************************************************************************************
void ChessNeuNeu::init(const PlayerType white, const PlayerType black)
{
  //FIXME players[Color::White].reset(createPlayer(white, Color::White));
  //FIXME players[Color::Black].reset(createPlayer(black, Color::Black));
  players[Color::White] = createPlayer(white, Color::White);
  players[Color::Black] = createPlayer(black, Color::Black);

  // Be sure to play with Kings (chessboard with no Kings is only
  // used for Neural trainings and unit tests).
  assert(false == rules.hasNoKing);

  // Debug
  std::cout
    << players[Color::White]->side()
    << " color is played by: "
    << players[Color::White]->type()
    << std::endl
    << players[Color::Black]->side()
    << " color is played by: "
    << players[Color::Black]->type()
    << std::endl << std::endl
    << rules.m_board << std::endl
    << rules.m_side << " are thinking ... "
    << std::flush;
}

// ***********************************************************************************************
//! \brief Parser the command-line option
// ***********************************************************************************************
static void configure_parser(cli::Parser& parser)
{
  parser.set_optional<std::string>
    ("w", "white", "human", "Define the white player: human | stockfish | neuneu");
  parser.set_optional<std::string>
    ("b", "black", "stockfish", "Define the black player: human | stockfish | neuneu");
  parser.set_optional<std::string>
    ("f", "fen", "", "Forsyth-Edwards notation");
}

// ***********************************************************************************************
//! \brief
// ***********************************************************************************************
int main(int argc, char** argv)
{
  // Initialize random seed
  srand(time(NULL));

  // Initialize the parser of command-line options
  cli::Parser parser(argc, argv);
  configure_parser(parser);
  parser.run_and_exit_if_error();

  try
    {
      //
      std::unique_ptr<ChessNeuNeu> chess;

      // Get Player types from command-line options --white and --black.
      // An exception is thrown if player type is badly typed.
      PlayerType Whites = playerType(parser.get<std::string>("w"));
      PlayerType Blacks = playerType(parser.get<std::string>("b"));

      // Optional: start the game with a given chessboard with the
      // comand-line --fen (Forsyth-Edwards notation).
      std::string fen = parser.get<std::string>("f");
      if (fen.empty())
        {
          chess = std::make_unique<ChessNeuNeu>(Whites, Blacks);
        }
      else
        {
          chess = std::make_unique<ChessNeuNeu>(Whites, Blacks, fen);
        }

      // Launch the GUI thread which will also start the game logic thread
      chess->loop(new Board(*chess, chess->rules, chess->players));
    }
  catch (std::string const& e)
    {
      std::cerr << "Fatal: " << e << std::endl;
      return 1;
    }

  return 0;
}
