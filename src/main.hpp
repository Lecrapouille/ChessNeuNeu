#ifndef MAIN_HPP
#  define MAIN_HPP

#  include "GUI.hpp"
#  include "Board.hpp"
#  include "Stockfish.hpp"
#  include "tcsp.hpp"
#  include "NeuNeu.hpp"
#  include "Human.hpp"
#  include "cmdparser.hpp"

class NeuNeuGUI: public GUIState
{
public:

  NeuNeuGUI(GUI* gui, cli::Parser& parser);
  void play(IPlayer *player, const Color color);

  virtual void draw(const float dt) override;
  virtual void update(const float dt) override;
  virtual void handleInput() override;
  IPlayer *PlayerFactory(const PlayerType type, const Color side);

  GUI* m_gui;
  Rules m_rules;
  Board m_board;
  IPlayer* m_players[2];
  Status m_previous_status = Status::Playing;
};

#endif
