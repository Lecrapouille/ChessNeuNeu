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

#include "TSCP.hpp"
#include <algorithm> // std::min

// FIXME TSCP has a bug: it misses flushing code therefore complete
// message cannot be received. So with version 181 you have to add
// some "fflush(stdout);" on its code (after printf() function not
// using "\n".
Tscp::Tscp(const Rules &rules, const Color side)
    : IPC("tscp"), IPlayer(PlayerType::TscpIA, side), m_rules(rules)
{
    // Force TSCP to move
    if (Color::White == side)
    {
        write("on\n");
    }
}

Tscp::~Tscp()
{
    write("bye\n");
}

void Tscp::abort()
{
    m_aborting = true;
}

std::string Tscp::play()
{
    // Extract the last move
    std::string last_move(m_rules.m_moved.substr(m_rules.m_moved.rfind(" ") + 1));

    // Force TSCP to move at first iteration
    if ((m_rules.m_moved.size() <= 4) && (Color::Black == side()))
    {
        last_move += " on";
    }

    // Create and send the command to TSCP
    write(last_move + '\n');

    // Get and parse the TSCP answer
    std::string answer;
    std::string move;
    uint8_t retry = 0u;
    int found;

    do
    {
        // The player wants to quit the game (from GUI) while we are
        // parsing TSCP answer ?
        if (m_aborting)
            goto l_quit;

        // Read TSCP message from the IPC pipe
        if (!read(answer))
            goto l_error;

        // Look for the keyword giving the move
        usleep(10000);
        found = answer.find("move: ");

        if (found >= 0)
        {
            // Keyword found ! Try extracting the move
            // 6 == strlen("move: ")
            // 5 == chess move with promotion or white space
            //      (ie: "e2e4 " or "e7e8q").
            move = answer.substr(found + 6, 5);

            // TSCP did not finish sending all its message: keep
            // looping.
            if (move.length() < 5)
                found = -1;
        }

        // No move found: wait a little to let TSCP returning a bigger
        // message and try again to extract the move.
        if  (-1 == found)
        {
            // Give time to TSCP to send more message
            usleep(10000);

            // Too many failures: abort
            if (++retry > 100)
                goto l_error;
        }
    }
    while (-1 == found);

    // Check if the returned move is well formed.
    assert(
        // Stalemate case
        (move == "(no l") ||
        // Regexp of move [a-h][1-8][a-h][1-8]
        ((move[0] >= 'a') && (move[0] <= 'h') &&
         (move[1] >= '1') && (move[1] <= '8') &&
         (move[2] >= 'a') && (move[2] <= 'h') &&
         (move[3] >= '1') && (move[3] <= '8') &&
         // Regexp of pawn promotion or white space
         ((move[4] == ' ') || (move[4] == '\n') ||
          (move[4] == 'n') || (move[4] == 'b') ||
          (move[4] == 'q') || (move[4] == 'r')))
           );

    // TSCP returns "(no legal move)" for stalemate
    if (move[0] == '(')
        return Move::none;

    // Chess move extracted with success
    return move;

l_quit:
    return IPlayer::quitting;

l_error:
    std::cerr << "Failed reading TSCP move" << std::endl;
    return IPlayer::error;
}
