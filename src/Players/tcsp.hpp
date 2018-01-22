#ifndef TCSP_HPP
#  define TCSP_HPP

#  include "IPlayer.hpp"
#  include "Rules.hpp"
#  include "IPC.hpp"

class Tcsp: public IPC, public IPlayer
{
public:

  Tcsp(const Rules &rules, const Color side);
  ~Tcsp();
  virtual std::string nextMove() override;

private:

  const Rules &m_rules;
  bool m_send_on;
};

#endif
