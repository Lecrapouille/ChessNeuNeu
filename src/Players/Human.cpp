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

#include "Human.hpp"

Human::Human(Rules const &rules, Color const side)
  : IPlayer(PlayerType::HumanPlayer, side), m_rules(rules)
{
}

Human::~Human()
{
}

void Human::abort()
{
  notified(IPlayer::error);
}

void Human::notified(std::string const& move)
{
  {
    std::unique_lock<std::mutex> mlock(m_mutex);
    m_move = move;
  }
  m_cond.notify_one();
}

// Tricky hack to get the move from the GUI.
std::string Human::play()
{
  std::unique_lock<std::mutex> mlock(m_mutex);

  m_cond.wait(mlock);
  std::string move(m_move);
  return move;
}
