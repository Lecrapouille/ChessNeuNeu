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

#ifndef HUMAN_HPP
#  define HUMAN_HPP

#  include "Player.hpp"
#  include <mutex>
#  include <condition_variable>

// ***********************************************************************************************
//! \brief Implement a chess player. The user uses the GUI for moving pieces.
//! Contrary to external software where we use bidir pipe, here the difficulty
//! is to synchronize the GUI thread events with this thread. So we use here
//! std::condition_variable.
// ***********************************************************************************************
class Human: public IPlayer
{
public:

  Human(const Rules &rules, const Color side);
  ~Human();
  virtual std::string play() override;
  virtual void abort() override;

  //! \brief Notification from the GUI.  When the mouse released event
  //! occured The human has made a valid move and we need to get it.
  void notified(std::string const& move);

private:

  const Rules &m_rules;
  //using MuxGuard = std::lock_guard<std::mutex>;
  //mutable std::mutex m_lock;
  std::mutex m_mutex;
  std::condition_variable m_cond;
  std::string m_move;
};

#endif
