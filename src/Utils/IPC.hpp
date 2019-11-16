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

#  include "Utils/Execve.hpp"

// ***********************************************************************************************
//! \brief Implement a Inter Process Communication based on Linux pipes. Be careful if process A
//! is expects B will send to it an message and B is expected A will send it a message. Both
//! processes will loop indefinitely.
// ***********************************************************************************************
class IPC
{
public:

  // ----------------------------------------------------------------------------
  //! \brief Constructor. Nothing is made.
  // ----------------------------------------------------------------------------
  IPC()
  {}

  // ----------------------------------------------------------------------------
  //! \brief Destructor. Terminate the communication with the parent process.
  // ----------------------------------------------------------------------------
  ~IPC()
  {}

  // ----------------------------------------------------------------------------
  //! \brief Terminate the communication with the parent process.
  // ----------------------------------------------------------------------------
  void close()
  {
    m_exec.close();
  }

  // ----------------------------------------------------------------------------
  //! \brief Create a bidirectional pipe and call a Linux command for starting
  //! the process that we want to communicate with.
  // ----------------------------------------------------------------------------
  bool operator()(std::string const& command)
  {
    const char* const arglist[] = { command.c_str(), nullptr };
    const char* const env[] = { nullptr };

    if (!m_exec(command.c_str(), arglist, env))
      return false;

    return setNonBlockingRead();
  }

  //! \brief Send a message to the external process.
  bool write(std::string const& msg);

  //! \brief Receive a message from the external process.
  bool read(std::string& msg);

private:

  //! \brief Force to non-blocking read
  bool setNonBlockingRead();

private:

  Execve m_exec;
};

#endif
