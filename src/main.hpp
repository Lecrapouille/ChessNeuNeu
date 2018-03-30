#ifndef MAIN_HPP
#  define MAIN_HPP

#  include "GUI/GUI.hpp"
#  include "Chess/Rules.hpp"
#  include "Players/Player.hpp"

// ***********************************************************************************************
//! \brief Main window showing the chessboard and allowing two players to play chess on this board.
// ***********************************************************************************************
class ChessNeuNeu : public Application
{
public:

  ChessNeuNeu(const PlayerType Whites, const PlayerType Blacks);

  //! \brief Create the GUI and and the start a t
  void run();

  inline Rules& rules()
  {
    return m_rules;
  }

  //FIXME private:

  void play();
  IPlayer *createPlayer(const PlayerType type, const Color side);

  Rules                    m_rules;
  //std::unique_ptr<IPlayer> m_players[2];
  IPlayer* m_players[2];
};

#endif
