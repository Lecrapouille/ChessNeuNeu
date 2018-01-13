#include "Stockfish.hpp"

Stockfish::Stockfish(const States &states)
  : m_states(states)
{
}

Stockfish::~Stockfish()
{
}

std::string Stockfish::nextMove()
{
  std::string position("position startpos moves ");
  position += m_states.moves;
  position += "\ngo\n";

  return "d7d5";
}
