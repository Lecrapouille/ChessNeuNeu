#ifndef STOCKFISH_HPP
#  define STOCKFISH_HPP

#  include "IA.hpp"
#  include "States.hpp"
#  include "IPC.hpp"

class Stockfish: public IPC, public IIA
{
public:

  Stockfish(const States &states);
  ~Stockfish();
  virtual std::string nextMove() override;
  void debug();

private:

  const States &m_states;
};

#endif
