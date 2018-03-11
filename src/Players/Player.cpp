#include "Player.hpp"
#include <iostream>
#include <algorithm>

static const char *c_player_names[] =
  {
    [PlayerType::HumanPlayer] = "Human",
    [PlayerType::StockfishIA] = "Stockfish",
    [PlayerType::TscpIA] = "TSCP",
    [PlayerType::NeuNeuIA] = "NeuNeu"
  };

const char *playerName(const PlayerType p)
{
  return c_player_names[p];
}

PlayerType playerType(const std::string& player)
{
  std::string name(player);
  std::transform(name.begin(), name.end(), name.begin(), ::tolower);
  if (name == "stockfish") return PlayerType::StockfishIA;
  if (name == "neuneu") return PlayerType::NeuNeuIA;
  if (name == "human") return PlayerType::HumanPlayer;
  if (name == "tscp") return PlayerType::TscpIA;

  throw std::string("Unknonw PlayerType");
}

std::ostream& operator<<(std::ostream& os, const PlayerType& p)
{
  os << c_player_names[p];
  return os;
}
