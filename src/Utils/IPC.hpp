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

#ifndef IPC_HPP
#  define IPC_HPP

#  include <unistd.h>
#  include <sys/wait.h>
//#  include <sys/prctl.h>
#  include <signal.h>
#  include <stdlib.h>
#  include <string>

class IPC
{
public:

  IPC(const std::string& command);
  ~IPC();

  int write(const std::string& msg);
  std::string read();

private:

  int open(const std::string& command);
  void close();

  pid_t m_pid;
  int m_wfd;
  int m_rfd;
};

#endif
