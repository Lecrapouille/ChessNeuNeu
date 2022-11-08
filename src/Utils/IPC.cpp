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
#include <vector>
#include <stdexcept>
#include <stdio.h>
#include <string.h>

IPC::IPC(std::string const& command)
{
    if (!open(command))
    {
        throw std::invalid_argument("Failed Creating bidirectional pipe");
    }
}

IPC::~IPC()
{
    close();
}

bool IPC::open(std::string const& command)
{
    std::cout << "Opening IPC with " << command
              << " ..." << std::endl;
    int status;
    int wpipe[2];
    int rpipe[2];

    std::vector<char*> argc{
        const_cast<char*>(command.c_str()),
        nullptr
    };

    //fflush(stdout);

    if (pipe(wpipe) || pipe(rpipe))
    {
        std::cerr << "Pipe creation failed" << std::endl;
        return false;
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
        return false;

    case 0: // child
        dup2(wpipe[0], STDIN_FILENO);
        dup2(rpipe[1], STDOUT_FILENO);

        ::close(wpipe[1]);
        ::close(rpipe[0]);

        // Ask kernel to deliver SIGTERM in case the parent dies
        //prctl(PR_SET_PDEATHSIG, SIGTERM);
        if (execvp(command.c_str(), argc.data()) == -1)
        {
            std::cerr << "execl failed. Reason: "
                      << strerror(errno) << std::endl;
        }

        // Check if child succeed
        m_pid = wait(&status);
        if (WIFEXITED(status))
        {
            std::cerr << "PID " << m_pid << " exited with status "
                      << WEXITSTATUS(status) << std::endl;
            return false;
        }
        else
        {
            std::cerr << "PID " << m_pid << " exited abnormally"
                      << std::endl;
            return false;
        }

        // FIXME never preinted
        std::cerr << "Child forked process eneded" << std::endl;
        exit(1);
        break;

    default: // Parent
        std::cout << "PID created " << m_pid << std::endl;

        ::close(wpipe[0]);
        ::close(rpipe[1]);

        // Force to non-blocking read
        int flags = fcntl(rpipe[0], F_GETFL, 0);
        int retval = fcntl(rpipe[0], F_SETFL, flags | O_NONBLOCK);
        if (retval < 0)
        {
            std::cerr << "IPC: failed configuring non-blocking read: "
                      << retval << std::endl;
        }
        m_wfd = wpipe[1];
        m_rfd = rpipe[0];
        return true;
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

int IPC::write(std::string const& msg)
{
    return ::write(m_wfd, msg.c_str(), msg.size());
}

bool IPC::read(std::string& msg)
{
    char buffer[128];
    int nb;

    msg.clear();
    do {
        nb = ::read(m_rfd, buffer, 127);
        if (nb > 0)
        {
            buffer[nb] = '\0';
            msg += (char*) buffer;
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
