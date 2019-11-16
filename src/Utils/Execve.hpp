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

#ifndef EXCEVE_HPP
#  define EXCEVE_HPP

#  include <iostream>
#  include <sys/types.h>

// -----------------------------------------------------------------------------
//! \brief Class wrapping a securized version of execve().
//!
//! Before calling execve() this class allows:
//!  - to closed file descriptors of the child process.
//!  - to drop user and group privileges.
//!  - to generate once again the seed generator for each process.
// -----------------------------------------------------------------------------
class Execve
{
public:

  // ---------------------------------------------------------------------------
  //! \brief Constructor. Nothing is made here.
  // ---------------------------------------------------------------------------
  Execve()
  {}

  // ---------------------------------------------------------------------------
  //! \brief Destructor. End the communication with
  // ---------------------------------------------------------------------------
  ~Execve()
  {
    close();
  }

  // ---------------------------------------------------------------------------
  //! \brief Wrapper for calling the c function execve from the forked child.
  // ---------------------------------------------------------------------------
  template <size_t N, size_t M>
  bool operator()(const char* path, const char* const (&argv)[N], const char *const (&envp)[M])
  {
    if ((N <= 0) || (argv[N - 1u] != nullptr))
      {
        std::cerr << "Malformed argv (expected a non dummy char* array ended by nullptr)" << std::endl;
        return false;
      }

    if ((M <= 0) || (envp[M - 1u] != nullptr))
      {
        std::cerr << "Malformed envp (expected a non dummy char* array ended by nullptr)" << std::endl;
        return false;
      }

    return this->open(path, const_cast<char* const*>(argv), const_cast<char* const*>(envp));
  }

  // ---------------------------------------------------------------------------
  //! \brief
  // ---------------------------------------------------------------------------
  bool close();

  // ---------------------------------------------------------------------------
  //! \brief Check if states of this class are correctly init.
  // ---------------------------------------------------------------------------
  inline bool isValid() const
  {
    return (m_child_pid >= 0) && (m_read_fd >= 0) && (m_write_fd >= 0);
  }

  // ---------------------------------------------------------------------------
  //! \brief Accessor. Return the file descriptor for reading.
  // ---------------------------------------------------------------------------
  inline int getRfd() const
  {
    return m_read_fd;
  }

  // ---------------------------------------------------------------------------
  //! \brief Accessor. Return the file descriptor for writing.
  // ---------------------------------------------------------------------------
  inline int getWfd() const
  {
    return m_write_fd;
  }

  // ---------------------------------------------------------------------------
  //! \brief Accessor. Return the PID of the child process.
  // ---------------------------------------------------------------------------
  inline int getPID() const
  {
    return m_child_pid;
  }

private:

  // ---------------------------------------------------------------------------
  //! \brief Open the pipe, forh this process. In the child process, execute the
  //! command passed as parameter. The parent process keeps its live.
  //!
  //! \param[in] path the command to execute.
  //! \prama[in] argv the parameterss of the command.
  //! \param[in] envp the environement path.
  //!
  //! \return true by the parent process if everything has been done with success.
  //! Else return false if something wrong happened when preparing the execve().
  //! Else exit(0) if execve() ends correctly or exit(127) if execve() could not
  //! execute its work.
  // ---------------------------------------------------------------------------
  bool open(const char *path, char *const argv[], char *const envp[]);

private:

  // \brief Blocking file descriptor for reading
  int   m_read_fd = -1;
  // \brief file descriptor for writting
  int   m_write_fd = -1;
  // \brief PID of the child process
  pid_t m_child_pid = -1;
};

#endif
