#ifndef MAIN_HPP
#  define MAIN_HPP

#  include "GUI.hpp"
#  include "Board.hpp"
#  include "Player.hpp"
#  include "CmdParser/cmdparser.hpp"

// ***********************************************************************************************
//! \brief Main window showing the chessboard and allowing two players to play chess on this board.
// ***********************************************************************************************
class ChessGame: public GUIState
{
public:

  ChessGame(GUI* gui, cli::Parser& parser);

private:

  void play(IPlayer& player, const Color color);
  void debug();

  virtual void draw(const float dt) override;
  virtual void update(const float dt) override;
  virtual void handleInput() override;
  IPlayer *PlayerFactory(const PlayerType type, const Color side);

  GUI* m_gui;
  Rules m_rules;
  Board m_board;
  std::unique_ptr<IPlayer> m_players[2];
  Status m_previous_status = Status::Playing;
};

// ***********************************************************************************************
//! \brief Window allowing to the user to choose the piece to promote.
// ***********************************************************************************************
//TODO class PawnPromotion: public GUIState

#endif
