#ifndef STOCKFISH_HPP
#  define STOCKFISH_HPP

#  include "IPlayer.hpp"
#  include "Rules.hpp"
#  include "IPC.hpp"

class Stockfish: public IPC, public IPlayer
{
public:

  Stockfish(const Rules &rules, const Color side);
  ~Stockfish();
  virtual std::string nextMove() override;
  void debug();

private:

  const Rules &m_rules;
};

#endif
