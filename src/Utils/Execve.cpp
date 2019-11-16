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

// ----------------------------------------------------------------------------
// This file has been inspired by:
// https://www.oreilly.com/library/view/secure-programming-cookbook/0596003943/
// The code is different because I noticed some errors
// ----------------------------------------------------------------------------

#include "Utils/Execve.hpp"

#include <sys/param.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <grp.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#ifndef DEV_NULL
#  define DEV_NULL "/dev/null"
#endif

// ----------------------------------------------------------------------------
//! \brief restore inherited privileges to the forked process which usually
//! will call execve.
// ----------------------------------------------------------------------------
static bool drop_privileges()
{
  gid_t newgid = getgid(), oldgid = getegid();
  uid_t newuid = getuid(), olduid = geteuid();

  // If root privileges are to be dropped, be sure to pare down the ancillary
  // groups for the process before doing anything else because the setgroups()
  // system call requires root privileges.  Drop ancillary groups regardless of
  // whether privileges are being dropped temporarily or permanently.
  if (!olduid)
    setgroups(1, &newgid);

  if (newgid != oldgid)
    {
      setegid(newgid);
      if (setgid(newgid) == -1)
        {
          std::cerr << "FAILED setting gid. Reason '"
                    << strerror(errno) << "'" << std::endl;
          return false;
        }
    }

    if (newuid != olduid)
      {
        seteuid(newuid);
        if (setuid(newuid) == -1)
          {
            std::cerr << "FAILED setting uid. Reason '"
                      << strerror(errno) << "'" << std::endl;
            return false;
          }
      }

    // Verify that the changes were successful
    if (newgid != oldgid && (setegid(oldgid) != -1 || getegid() != newgid))
      {
        std::cerr << "FAILED changing gid. Reason '"
                  << strerror(errno) << "'" << std::endl;
        return false;
      }

    if (newuid != olduid && (seteuid(olduid) != -1 || geteuid() != newuid))
      {
        std::cerr << "FAILED changing uid. Reason '"
                  << strerror(errno) << "'" << std::endl;
        return false;
      }

    //LOGD("OK %d %d -- %d %d", oldgid, newgid, olduid, newuid);
    return true;
}

// ----------------------------------------------------------------------------
//! \brief Check if stdout, stdin, stderr are opened else reopen them to /dev/null
//! \return fd file descriptor for stdout, stdin, stdcerr and shall be 0 or 1 or 2.
// ----------------------------------------------------------------------------
static bool open_devnull(int const fd)
{
  FILE *f = nullptr;

  if (!fd) f = freopen(DEV_NULL, "rb", stdin);
  else if (fd == 1) f = freopen(DEV_NULL, "wb", stdout);
  else if (fd == 2) f = freopen(DEV_NULL, "wb", stderr);
  return (f && fileno(f) == fd);
}

// ----------------------------------------------------------------------------
//! \brief Replace the deprecated POSIX function getdtablesize()
//! \return the number of file descriptors that the system can manage.
// ----------------------------------------------------------------------------
static int get_table_size()
{
  struct rlimit r;
  int fds;

  if (getrlimit(RLIMIT_NOFILE, &r) < 0)
    {
      fds = static_cast<int>(sysconf(_SC_OPEN_MAX));
      if (fds == -1)
        fds = 1024;
    }
  else
    {
      fds = static_cast<int>(r.rlim_cur);
    }

  return fds;
}

// ----------------------------------------------------------------------------
//! \brief Closed opened file descriptors except fd used for pipes, stdout,
//! stdin, stderr.
//!
//! \note You can see the list of opened file descriptors with:
//! ls /proc/getpid()/fd/
//!
//! \param[in] wpipe file descriptors of the pipe to preserve.
//! \param[in] rpipe file descriptors of the pipe to preserve.
//! \return true is sanitaze has been done with success.
// ----------------------------------------------------------------------------
static bool sanitize_files(int const wpipe[2], int const rpipe[2])
{
  bool res = true;
  int fd, fds;
  struct stat st;

  // Make sure all open descriptors other than the standard ones are closed
  fds = get_table_size();

  for (fd = 3; fd <= fds; ++fd)
    {
      // Do not close fd of pipes: they will be used for execve()
      if ((fd != wpipe[0]) && (fd != wpipe[1]) &&
          (fd != rpipe[0]) && (fd != rpipe[1]))
        {
          ::close(fd);
        }
    }

  // Verify that the standard descriptors are open. If they're not, attempt to
  // open them using /dev/null.  If any are unsuccessful, abort.
  for (fd = 0; fd < 3; ++fd)
    {
      if (fstat(fd, &st) == -1 && (errno != EBADF || !open_devnull(fd)))
        res = false;
    }

  return res;
}

// ----------------------------------------------------------------------------
//! \brief Change the seed for random numbers. This avoids that the parent and
//! the forked child have the same seed.
// ----------------------------------------------------------------------------
static void reseed()
{
  struct timespec tm;
  clock_gettime(CLOCK_MONOTONIC, &tm);
  unsigned t = (unsigned) (tm.tv_sec ^ tm.tv_nsec ^ (tm.tv_nsec >> 31));
  srand(t ^ (((unsigned) getpid()) << 16));
}

// ----------------------------------------------------------------------------
bool Execve::open(const char *path, char *const argv[], char *const envp[])
{
  int wpipe[2], rpipe[2];

  m_read_fd = m_write_fd = m_child_pid = -1;

  if (pipe(wpipe) == -1)
    {
      std::cerr << "secure_popen: failed pip stdin. Reason '"
                << strerror(errno) << "'" << std::endl;
      return false;
    }

  if (pipe(rpipe) == -1)
    {
      std::cerr << "secure_popen: failed pip stdout. Reason '"
                << strerror(errno) << "'" << std::endl;
      ::close(wpipe[1]);
      ::close(wpipe[0]);
      return false;
    }

  m_child_pid = ::fork();
  switch (m_child_pid)
    {
    case -1:
      std::cerr << "secure_popen: failed forking. Reason '"
                << strerror(errno) << "'" << std::endl;
      ::close(rpipe[0]);
      ::close(rpipe[1]);
      ::close(wpipe[0]);
      ::close(wpipe[1]);
      return false;

    case 0: // Child proces
      if (wpipe[0] != STDIN_FILENO)
        {
          ::dup2(wpipe[0], STDIN_FILENO);
          ::close(wpipe[0]);
        }

      if (rpipe[1] != STDOUT_FILENO)
        {
          ::dup2(rpipe[1], STDOUT_FILENO);
          ::close(rpipe[1]);
        }

      ::close(wpipe[1]);
      ::close(rpipe[0]);

      sanitize_files(wpipe, rpipe);
      drop_privileges();
      reseed();

      //__gcov_flush();
      if (::execve(path, argv, envp) < 0)
        {
          std::cerr << "execve: failed exec '" << path
                    << "'. Reason '" << strerror(errno)
                    << "'" << std::endl;
        }
      exit(127);

    default: // Parent process
      reseed();

      ::close(wpipe[0]);
      ::close(rpipe[1]);

      m_write_fd = wpipe[1];
      m_read_fd = rpipe[0];

      return true;
    }
}

// ----------------------------------------------------------------------------
bool Execve::close()
{
  int   status = 0;
  pid_t pid = 0;

  std::cerr << "Halting execve PID " << m_child_pid << std::endl;
  if (m_child_pid != -1)
    {
      do {
        pid = waitpid(m_child_pid, &status, 0);
      } while (pid == -1 && errno == EINTR);
    }

  if (m_read_fd)
    ::close(m_read_fd);

  if (m_write_fd)
    ::close(m_write_fd);

  if (pid != -1 && WIFEXITED(status))
    return WEXITSTATUS(status);

  m_read_fd = m_write_fd = m_child_pid = -1;
  return pid != -1;
}
