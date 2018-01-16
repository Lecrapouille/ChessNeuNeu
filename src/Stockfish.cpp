#include "Stockfish.hpp"
#include <iostream>

Stockfish::Stockfish(const States &states)
  : IPC("stockfish"), m_states(states)
{
  std::cout << read() << std::endl;
}

Stockfish::~Stockfish()
{
}

std::string Stockfish::nextMove()
{
  std::string position("position startpos moves ");
  position += m_states.moves;
  position += "\ngo\n";
  write(position);
  usleep(50000);

  std::string answer = read();
  int n = answer.find("bestmove");
  if (n != -1)
    {
      std::string move = answer.substr(n + 9, 4);
      //std::cout << "Next move is '" << move << "'" << std::endl;
      return move;
    }

  std::cout << "error" << std::endl;
  return "error";
}

void Stockfish::debug()
{
  write("d\n");
  std::cout << read() << std::endl;
}
