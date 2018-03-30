#ifndef PLAYER_HPP
#  define PLAYER_HPP

#  include "Chess/Pieces.hpp"
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

  //! \brief Compute and return a legal move.
  //! This method can take long minutes it's ok
  //! because this method should be called from
  //! a thread.
  virtual std::string play() = 0;

  //! \bref Abort signal for halting properlu play()
  virtual void abort() = 0;

  inline Color side() const
  {
    return m_side;
  }

  inline PlayerType type() const
  {
    return m_type;
  }

  static constexpr const char* error = "error";

private:

  PlayerType m_type;
  Color m_side;
};

std::ostream& operator<<(std::ostream& os, const PlayerType& p);
const char *playerName(const PlayerType p);
PlayerType playerType(const std::string& player);

#endif
