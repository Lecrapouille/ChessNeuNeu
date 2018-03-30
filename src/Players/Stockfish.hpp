#ifndef STOCKFISH_HPP
#  define STOCKFISH_HPP

#  include "Player.hpp"
#  include "Chess/Rules.hpp"
#  include "Utils/IPC.hpp"

class Stockfish: public IPC, public IPlayer
{
public:

  Stockfish(const Rules &rules, const Color side);
  ~Stockfish();
  virtual std::string play() override;
  virtual void abort() override;
  void debug();

private:

  const Rules &m_rules;
  bool m_aborting = false;
};

#endif
