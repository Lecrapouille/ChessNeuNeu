#ifndef PLAYER_HPP
#  define PLAYER_HPP

#  include "Pieces.hpp"
#  include <string>

// ***********************************************************************************************
//! \brief Define here
// ***********************************************************************************************
enum PlayerType { HumanPlayer, StockfishIA, TscpIA, NeuNeuIA };

// ***********************************************************************************************
//! \brief Interface class for a chess player.
// ***********************************************************************************************
class IPlayer
{
public:

  IPlayer(const PlayerType type, const Color side)
    : m_type(type), m_side(side)
  {
  }

  virtual ~IPlayer()
  {
  }

  //! \brief Return a legal move.
  virtual std::string play() = 0;

  inline Color side() const
  {
    return m_side;
  }

  inline PlayerType type() const
  {
    return m_type;
  }

private:

  PlayerType m_type;
  Color m_side;
};

std::ostream& operator<<(std::ostream& os, const PlayerType& p);
const char *playerName(const PlayerType p);
PlayerType playerType(const std::string& player);

#endif
