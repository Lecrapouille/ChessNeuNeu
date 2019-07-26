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

#include "IPC.hpp"
#include <fcntl.h>
#include <iostream>
#include <stdexcept>

IPC::IPC(std::string const& command)
{
  if (open(command))
    {
      throw std::invalid_argument("Failed Creating bidirectional pipe");
    }
}

IPC::~IPC()
{
  close();
}

int IPC::open(std::string const& command)
{
  int wpipe[2];
  int rpipe[2];

  //fflush(stdout);

  if (pipe(wpipe) || pipe(rpipe))
    {
      std::cerr << "Pipe creation failed" << std::endl;
      return 1;
    }

  m_pid = fork();
  switch (m_pid)
    {
    case -1:
      std::cerr << "fork failed" << std::endl;

      ::close(wpipe[0]);
      ::close(wpipe[1]);
      ::close(rpipe[0]);
      ::close(rpipe[1]);
      return 1;

    case 0: // child
      dup2(wpipe[0], STDIN_FILENO);
      dup2(rpipe[1], STDOUT_FILENO);
      dup2(rpipe[1], STDERR_FILENO);

      ::close(wpipe[1]);
      ::close(rpipe[0]);

      // Ask kernel to deliver SIGTERM in case the parent dies
      //prctl(PR_SET_PDEATHSIG, SIGTERM);
      if (-1 == execl("/bin/sh", "sh", "-c", command.c_str(), (char *) NULL))
        {
          std::cerr << "execl failed" << std::endl;
        }

      // Nothing below this line should be executed by child process. If so,
      // it means that the execl function wasn't successfull, so lets exit:
      exit(1);

    default: // Parent
      ::close(wpipe[0]);
      ::close(rpipe[1]);

      // Force to non-blocking read
      int flags = fcntl(rpipe[0], F_GETFL, 0);
      int retval = fcntl(rpipe[0], F_SETFL, flags | O_NONBLOCK);
      if (retval < 0)
        {
          std::cerr << "IPC: failed configuring non-blocking read: " << retval << std::endl;
        }
      m_wfd = wpipe[1];
      m_rfd = rpipe[0];
      return 0;
    }
}

void IPC::close()
{
  int status;
  pid_t p;

  ::close(m_wfd);
  ::close(m_rfd);
  do {
    p = waitpid(m_pid, &status, 0);
  } while (p == -1 && errno == EINTR);
}

ssize_t IPC::write(std::string const& msg)
{
  return ::write(m_wfd, msg.c_str(), msg.size());
}

bool IPC::read(std::string& msg)
{
  char buffer[128];
  ssize_t nb;

  msg.clear();
  do {
    nb = ::read(m_rfd, buffer, 127);
    if (nb > 0)
      {
        buffer[nb] = '\0';
        msg += buffer;
      }

    // Non-blocking reading ==> Equivalent to nb == 0
    else if (errno == EWOULDBLOCK)
      {
        nb = 0;
      }

    // Failure
    if (nb < 0)
      {
        std::cerr << "IPC: Failed reading in pipe" << std::endl;
      }
  } while (nb > 0);

  // Read at least one char ?
  return (nb >= 0);
}
