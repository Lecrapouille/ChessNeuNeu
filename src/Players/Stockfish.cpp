#include "Stockfish.hpp"

Stockfish::Stockfish(const Rules &rules, const Color side)
  : IPC("stockfish"), IPlayer(PlayerType::StockfishIA, side), m_rules(rules)
{
  std::cout << read() << std::endl;
}

Stockfish::~Stockfish()
{
}

void Stockfish::abort()
{
  m_aborting = true;
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

      if (m_aborting)
        return IPlayer::error;
      return move;
    }

  // FIXME: a retenter
  return IPlayer::error;
}

void Stockfish::debug()
{
  write("d\n");
  std::cout << read() << std::endl;
}
