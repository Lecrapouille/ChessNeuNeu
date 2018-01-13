#ifndef STOCKFISH_HPP
#  define STOCKFISH_HPP

#  include "IA.hpp"
#  include "States.hpp"

class Stockfish: public IIA
{
public:

  Stockfish(const States &states);
  ~Stockfish();
  virtual std::string nextMove() override;

private:

  const States &m_states;
};

#endif
