#ifndef IPLAYER_HPP
#  define IPLAYER_HPP

#  include "Pieces.hpp"
#  include <string>

enum PlayerType { HumanPlayer, StockfishIA, TcspIA, NeuNeuIA };

inline std::ostream& operator<<(std::ostream& os, const PlayerType& p)
{
  switch (p)
    {
    case PlayerType::HumanPlayer:
      os << "Human";
      break;
    case PlayerType::StockfishIA:
      os << "Stockfish";
      break;
    case PlayerType::NeuNeuIA:
      os << "NeuNeu";
      break;
    case PlayerType::TcspIA:
      os << "tcsp";
      break;
    default:
      os << "Unknown";
      break;
    }

  return os;
}

class IPlayer
{
public:

  IPlayer(const PlayerType type, const Color side)
    : m_type(type), m_side(side)
  {
  }

  virtual ~IPlayer() {}
  virtual std::string nextMove() = 0;
  inline Color side() const
  {
    return m_side;
  }

  inline PlayerType type() const
  {
    return m_type;
  }

protected:

  PlayerType m_type;
  Color m_side;
};

#endif
