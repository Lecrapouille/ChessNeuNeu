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
