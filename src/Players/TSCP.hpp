#ifndef TSCP_HPP
#  define TSCP_HPP

#  include "Player.hpp"
#  include "Utils/IPC.hpp"

class Tscp: public IPC, public IPlayer
{
public:

  Tscp(const Rules &rules, const Color side);
  ~Tscp();
  virtual std::string play() override;
  virtual void abort() override;

private:

  const Rules &m_rules;
  bool m_send_on;
};

#endif
