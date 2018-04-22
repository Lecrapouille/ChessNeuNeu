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

Tscp::Tscp(const Rules &rules, const Color side)
  : IPC("tscp"), IPlayer(PlayerType::TscpIA, side), m_rules(rules)
{
  usleep(10000);
  read();
  if (Color::White == side)
    {
      write("on\n"); usleep(1000000);
      m_send_on = true;
    }
  else
    {
      m_send_on = false;
    }
  //std::cout << read() << std::endl;
}

Tscp::~Tscp()
{
  write("bye\n");
}

void Tscp::abort()
{
  //FIXME
}

std::string Tscp::play()
{
  //if (m_rules.m_moved.size() >= 5)
  std::string next_move(m_rules.m_moved.substr(m_rules.m_moved.size() - 5 , 4));
  std::cout << "prochain coup '" << next_move << "'" << std::endl;
  write(next_move + '\n');
  usleep(100000);

  if (m_send_on == false)
    {
      write("on\n");
      usleep(100000);
      m_send_on = true;
    }

  std::string answer = read();
  int n = answer.find("move: ");
  if (n != -1)
    {
      std::string move = answer.substr(n, 4);
      std::cout << "TSCP move is '" << move << "'" << std::endl;
      return move;
    }

  std::cout << "error" << std::endl;
  return IPlayer::error;
}
