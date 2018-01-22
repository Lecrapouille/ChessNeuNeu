#ifndef NEUNEU_HPP
#  define NEUNEU_HPP

#  include "IPlayer.hpp"
#  include "Rules.hpp"

class NeuNeu: public IPlayer
{
public:

  NeuNeu(const Rules &rules, const Color side);
  ~NeuNeu();
  virtual std::string nextMove() override;

private:

  const Rules &m_rules;
};

#endif
