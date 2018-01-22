#ifndef HUMAN_HPP
#  define HUMAN_HPP

#  include "IPlayer.hpp"
#  include "Board.hpp"

class Human: public IPlayer
{
public:

  Human(const Board &board, const Color side);
  ~Human();
  virtual std::string nextMove() override;

private:

  const Board &m_board;
};

#endif
