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

#include "Stockfish.hpp"

Stockfish::Stockfish(const Rules &rules, const Color side, std::string const& fen)
  : IPC("stockfish"),
    IPlayer(PlayerType::StockfishIA, side),
    m_fen(fen),
    m_rules(rules)
{
  std::cout << read() << std::endl;
}

Stockfish::~Stockfish()
{
}

void Stockfish::abort()
{
  m_aborting = true;
}

std::string Stockfish::play()
{
  // Command to send to Stockfish
  std::string command("position ");

  if (m_fen.empty())
    {
      // Initial chessboard
      command += "startpos";
    }
  else
    {
      // Load the chessboard with the Forsyth–Edwards notation
      command += "fen ";
      command += m_fen;
    }

  // Add the list of moves
  command += " moves ";
  command += m_rules.m_moved;
  command += "\ngo\n";

  // Send the command to Stockfish
  write(command);

  // FIXME: Give time to Stockfish
  usleep(100000);

  // Get and parse the Stockfish answer
  std::string answer = read();
  int n = answer.find("bestmove");
  if (n != -1)
    {
      std::string move = answer.substr(n + 9, 5);

      // Ctr-C signal
      if (m_aborting)
        return IPlayer::error;

      // Regexp of move [a-h][1-8][a-h][1-8][nbqr ]
      if ((move[0] >= 'a') && (move[0] <= 'h') &&
          (move[1] >= '1') && (move[1] <= '8') &&
          (move[2] >= 'a') && (move[2] <= 'h') &&
          (move[3] >= '1') && (move[3] <= '8') &&
          ((move[4] == ' ') || (move[4] == 'n') ||
           (move[4] == 'b') || (move[4] == 'q') ||
           (move[4] == 'r')))
        return move;

      // Stockfish returns "(none)" for stalemate
      return IPlayer::error;
    }

  // FIXME: Retry
  return IPlayer::error;
}

void Stockfish::debug()
{
  write("d\n");
  std::cout << read() << std::endl;
}
