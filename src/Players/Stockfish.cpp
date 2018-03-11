#include "Stockfish.hpp"
#include <iostream>

Stockfish::Stockfish(const Rules &rules, const Color side)
  : IPC("stockfish"), IPlayer(PlayerType::StockfishIA, side), m_rules(rules)
{
  std::cout << read() << std::endl;
}

Stockfish::~Stockfish()
{
}

std::string Stockfish::play()
{
  std::string position("position startpos moves ");
  position += m_rules.m_moved;
  position += "\ngo\n";
  write(position);

  // FIXME: a enlever
  usleep(100000);

  std::string answer = read();
  int n = answer.find("bestmove");
  if (n != -1)
    {
      std::string move = answer.substr(n + 9, 4);
      //std::cout << "Next move is '" << move << "'" << std::endl;
      return move;
    }

  // FIXME: a retenter
  std::cout << "error" << std::endl;
  return "error";
}

void Stockfish::debug()
{
  write("d\n");
  std::cout << read() << std::endl;
}
