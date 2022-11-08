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

#include "Loki.hpp"

//------------------------------------------------------------------------------
Loki::Loki(const Rules &rules, const Color side, std::string const& fen)
    : IPC(THIRDPART "/Loki/Loki3"),
      IPlayer(PlayerType::LokiIA, side),
      m_initial_board(fen),
      m_rules(rules)
{}

//------------------------------------------------------------------------------
void Loki::abort()
{
    m_aborting = true;
}

//------------------------------------------------------------------------------
std::string Loki::play()
{
    // Create the command to be sent to Loki
    std::string command("position ");

    if (m_initial_board.empty())
    {
        // Game started from the initial chessboard position
        command += "startpos";
    }
    else
    {
        // Game started from a loaded chessboard
        command += "fen ";
        command += m_initial_board;
    }

    // Append the list of moves
    command += " moves ";
    command += m_rules.m_moved;
    command += "\ngo depth 6\n";

    // Send the command to Loki
    write(command);

    // Get and parse the Loki answer
    std::string answer;
    std::string move;
    uint32_t retry = 0u;
    int found;

    do
    {
        // The player wants to quit the game (from GUI) while we are
        // parsing Loki answer ?
        if (m_aborting)
            goto l_quit;

        // Read Loki message from the IPC pipe
        if (!read(answer))
            goto l_error;

        // Look for the keyword giving the move
        found = answer.find("bestmove");

        if (found >= 0)
        {
            // Keyword found ! Try extracting the move
            // 9 == strlen("bestmove")
            // 5 == chess move with promotion or white space
            //      (ie: "e2e4 " or "e7e8q").
            move = answer.substr(found + 9, 5);

            // Loki did not finish sending all its message: keep
            // looping.
            if (move.length() < 5)
                found = -1;
        }

        // No move found: wait a little to let Loki returning a bigger
        // message and try again to extract the move.
        if  (-1 == found)
        {
            // Give time to Loki to send more message
            usleep(1000);

            // Too many failures: abort
            if (++retry > 1000)
                goto l_error;
        }
    }
    while (-1 == found);

    // Check if the returned move is well formed.
    if (!(
            // Stalemate case
            (move == "(none") ||
            // Regexp of move [a-h][1-8][a-h][1-8]
            ((move[0] >= 'a') && (move[0] <= 'h') &&
             (move[1] >= '1') && (move[1] <= '8') &&
             (move[2] >= 'a') && (move[2] <= 'h') &&
             (move[3] >= '1') && (move[3] <= '8') &&
             // Regexp of pawn promotion or white space
             ((move[4] == ' ') || (move[4] == '\n') ||
              (move[4] == 'n') || (move[4] == 'b') ||
              (move[4] == 'q') || (move[4] == 'r')))
          ))
    {
        goto l_error;
    }

    // Loki returns "(none)" for stalemate
    if (move[0] == '(')
        return Move::none;

    // Chess move extracted with success
    return move;

l_quit:
    return IPlayer::quitting;

l_error:
    std::cerr << "Failed reading Loki move '"
              << move << "'" << std::endl;
    return IPlayer::error;
}
