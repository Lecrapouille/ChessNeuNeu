#include "Human.hpp"
#include <iostream>

Human::Human(const Board &board, const Color side)
  : IPlayer(PlayerType::HumanPlayer, side), m_board(board)
{
}

Human::~Human()
{
}

// TODO: better integration with mouse click and Board
// but SFML does not like threads.
std::string Human::nextMove()
{
  return "error";
}
