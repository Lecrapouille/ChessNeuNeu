#ifndef HUMAN_HPP
#  define HUMAN_HPP

#  include "Player.hpp"
#  include "Board.hpp"

class Human: public IPlayer
{
public:

  Human(Board &board, const Color side);
  ~Human();
  virtual std::string play() override;

private:

  Board &m_board;
};

#endif
