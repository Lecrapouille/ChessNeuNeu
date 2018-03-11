#include "Human.hpp"
#include <iostream>

Human::Human(Board &board, const Color side)
  : IPlayer(PlayerType::HumanPlayer, side), m_board(board)
{
}

Human::~Human()
{
}

std::string Human::play()
{
  //while (false == m_board.releaseFigure())
  //  ;
  return "error";//m_board.lastMove();
}
