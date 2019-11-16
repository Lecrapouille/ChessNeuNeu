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

#include "Utils/IPC.hpp"
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

// ----------------------------------------------------------------------------
bool IPC::setNonBlockingRead()
{
  if (!m_exec.isValid())
    {
      std::cerr << "IPC: failed configuring non-blocking read. Reason 'badly opened pipe"
                << std::endl;
      return false;
    }

  int flags = fcntl(m_exec.getRfd(), F_GETFL, 0);
  int retval = fcntl(m_exec.getRfd(), F_SETFL, flags | O_NONBLOCK);
  if (retval < 0)
    {
      std::cerr << "IPC: failed configuring non-blocking read. Reason '"
                << strerror(errno) << "'" << std::endl;
      return false;
    }

  return true;
}

// ----------------------------------------------------------------------------
bool IPC::write(std::string const& msg)
{
  if (!m_exec.isValid())
    {
      std::cerr << "Tried to write to a badly opened pipe" << std::endl;
      return false;
    }

  //std::cout << "PIPE write '" << msg.c_str() << "'" << std::endl;
  ssize_t bytes = ::write(m_exec.getWfd(), msg.c_str(), msg.size());
  if (bytes != ssize_t(msg.size()))
    {
      std::cerr << "Failed writing " << bytes << " bytes. Reason '"
                << strerror(errno) << "'" << std::endl;
      return false;
    }

  return true;
}

// ----------------------------------------------------------------------------
bool IPC::read(std::string& msg)
{
  char buffer[128];
  int nb;

  msg.clear();
  do {
    nb = ::read(m_exec.getRfd(), buffer, 127);
    if (nb > 0)
      {
        buffer[nb] = '\0';
        msg += (char*) buffer;
      }

    // Non-blocking reading ==> Equivalent to nb == 0
    else if (errno == EWOULDBLOCK)
      nb = 0;

    // Failure
    if (nb < 0)
      std::cerr << "IPC: Failed reading in pipe" << std::endl;
  } while (nb > 0);

  // Read at least one char ?
  return (nb >= 0);
}
