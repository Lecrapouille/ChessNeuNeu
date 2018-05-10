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
  //std::cout << read() << std::endl;
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

  // Get and parse the Stockfish answer
  std::string answer;
  std::string move;
  uint8_t retry = 0u;
  int found;

  do
    {
      // ChessNeuNeu is quitting ?
      if (m_aborting)
        goto l_quit;

      // Read Stockfish message from the IPC
      if (!read(answer))
        goto l_error;

      // Look for a keyword
      found = answer.find("bestmove");

      // Found try extracting the move
      if (found >= 0)
        {
          // Extract move
          move = answer.substr(found + 9, 5);

          // Stockfish did not finish sending all its message
          std::cout << "Move '" << move << "' " << move.length() << std::endl;
          if (move.length() < 5)
            found = -1;
        }

      // No move found: wait a little to let Stockfish returning a bigger
      // message and try again to extract the move.
      if  (-1 == found)
        {
          // Give time to Stockfish to send more message
          usleep(1000);

          // Too many failures
          if (++retry > 100)
            goto l_error;
        }
    }
  while (-1 == found);

  std::cout << "Move '" << move << "'" << std::endl;
  assert(// Stalemate case
         (move == "(none") ||
         // Regexp of move [a-h][1-8][a-h][1-8]
         ((move[0] >= 'a') && (move[0] <= 'h') &&
          (move[1] >= '1') && (move[1] <= '8') &&
          (move[2] >= 'a') && (move[2] <= 'h') &&
          (move[3] >= '1') && (move[3] <= '8') &&
          // Regexp of
          ((move[4] == ' ') || (move[4] == '\n') ||
           (move[4] == 'n') || (move[4] == 'b') ||
           (move[4] == 'q') || (move[4] == 'r'))));

  // Stockfish returns "(none)" for stalemate
  if (move[0] == '(')
    return IPlayer::none;

  // Chess move extracted with success
  return move;

l_quit:
  return IPlayer::none;

l_error:
  std::cerr << "Failed reading Stockfish move" << std::endl;
  return IPlayer::error;
}

void Stockfish::debug()
{
  //write("d\n");
  //std::cout << read() << std::endl;
}
