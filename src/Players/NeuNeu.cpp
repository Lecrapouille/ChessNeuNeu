#include "NeuNeu.hpp"
#include <iostream>
#include <cstdlib>

NeuNeu::NeuNeu(const Rules &rules, const Color side)
  : IPlayer(PlayerType::NeuNeuIA, side), m_rules(rules)
{
}

NeuNeu::~NeuNeu()
{
}

std::string NeuNeu::nextMove()
{
  int len = m_rules.m_legal_moves.size();
  if (len == 0)
    return "error";

  int rn = rand() % len;

  return Move2str(m_rules.m_legal_moves[rn]);
}
