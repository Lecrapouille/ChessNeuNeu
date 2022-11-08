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

#ifndef IPC_HPP
#  define IPC_HPP

#  include <unistd.h>
#  include <sys/wait.h>
//#  include <sys/prctl.h>
#  include <signal.h>
#  include <stdlib.h>
#  include <string>

// *****************************************************************************
//! \brief Implement a generic bidirectional Inter Process Communication (IPC)
//! link based on Linux pipes. This allows two processes to talk (requests and
//! answers) but be careful: if the process A is expecting to receive a message
//! from the process B but B is expecting to receive a message from A, they will
//! both loop indefinitely. This class does not manage this case.
// *****************************************************************************
class IPC
{
public:

    //! \brief Constructor. Create a bidirectional pipe and call a Linux command
    //! for starting the process that we want to communicate with.
    IPC(std::string const& command);

    //! \brief Destructor. Close file descriptors of pipes.
    ~IPC();

    //! \brief Send a message to the external process.
    int write(std::string const& msg);

    //! \brief Receive a message from the external process.
    bool read(std::string& msg);

    //! \brief Return the PID of the process in communication
    inline int pid() const { return m_pid; }

private:

    bool open(std::string const& command);
    void close();

    pid_t m_pid;
    int m_wfd;
    int m_rfd;
};

#endif
