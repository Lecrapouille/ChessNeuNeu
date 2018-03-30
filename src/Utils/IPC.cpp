#include "IPC.hpp"
#include <fcntl.h>
#include <iostream>
#include <stdexcept>

IPC::IPC(const std::string& command)
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

int IPC::open(const std::string& command)
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
      int retval = fcntl(rpipe[0], F_SETFL, fcntl(rpipe[0], F_GETFL) | O_NONBLOCK);
      //std::cerr << "fcntl " << retval << std::endl;
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

int IPC::write(const std::string& msg)
{
  return ::write(m_wfd, msg.c_str(), msg.size());
}

char buffer[2048];
std::string IPC::read()
{
  std::string msg;
  int nb;

  do {
    nb = ::read(m_rfd, buffer, 1024);
    if (nb > 0)
      {
        buffer[nb] = '\0';
        msg += (char*) buffer;
      }
  } while (nb > 0);

  return msg;
}
