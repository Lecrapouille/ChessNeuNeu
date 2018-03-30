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

  //! \brief Constructor. Start with initial board and white to play.
  ChessNeuNeu(const PlayerType Whites, const PlayerType Blacks);

  //! \brief Constructor. Start with a given board using the Forsyth-Edwards notation.
  ChessNeuNeu(const PlayerType Whites, const PlayerType Blacks, std::string const& fen);

  //! \brief Create the GUI and and the start a t
  void run();

private:

  //! \brief Shall only be called by constructors. This method is here just
  //! for factorizing the code.
  void init(const PlayerType white, const PlayerType black);

  //! \brief Factory Create a player: human, IA with the desired color.
  IPlayer *createPlayer(const PlayerType type, const Color side);

public:

  //! \brief Save the chessboard passed in command-line option.
  //! This will be used for Stockfish.
  std::string m_fen;

  //! \the Chess referee and game states.
  Rules    rules;

  //! \brief The
  //FIXME std::unique_ptr<IPlayer> m_players[2];
  IPlayer* players[2];
};

#endif
