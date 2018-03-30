#include "main.hpp"
#include "GUI/Board.hpp"
#include "Players/Stockfish.hpp"
#include "Players/TSCP.hpp"
#include "Players/NeuNeu.hpp"
#include "Players/Human.hpp"
#include "CmdParser/cmdparser.hpp"

IPlayer *ChessNeuNeu::createPlayer(const PlayerType type, const Color side)
{
  switch (type)
    {
    case PlayerType::StockfishIA:
      return new Stockfish(m_rules, side);
    case PlayerType::TscpIA:
      return new Tscp(m_rules, side);
    case PlayerType::NeuNeuIA:
      return new NeuNeu(m_rules, side);
    case PlayerType::HumanPlayer:
      return new Human(m_rules, side);
    default:
      throw std::string("createPlayer: Unknown PlayerType");
      break;
    }
}

ChessNeuNeu::ChessNeuNeu(const PlayerType white, const PlayerType black)
{
  //FIXME m_players[Color::White].reset(createPlayer(white, Color::White));
  //FIXME m_players[Color::Black].reset(createPlayer(black, Color::Black));
  m_players[Color::White] = createPlayer(white, Color::White);
  m_players[Color::Black] = createPlayer(black, Color::Black);

  // Be sure to play with Kings (chessboard with no Kings is only
  // used for Neural trainings and unit tests).
  assert(false == m_rules.hasNoKing);

  // Debug
  std::cout
    << m_players[Color::White]->side()
    << " color is played by: "
    << m_players[Color::White]->type()
    << std::endl
    << m_players[Color::Black]->side()
    << " color is played by: "
    << m_players[Color::Black]->type()
    << std::endl << std::endl
    << m_rules.m_board << std::endl
    << m_rules.m_side << " are thinking ... "
    << std::flush;
}

static void configure_parser(cli::Parser& parser)
{
  parser.set_optional<std::string>
    ("w", "white", "Human", "Define the white player: Human | Stockfish | NeuNeu");
  parser.set_optional<std::string>
    ("b", "black", "Stockfish", "Define the black player: Human | Stockfish | NeuNeu");
}

int main(int argc, char** argv)
{
  int err = 0;

  // Initialize random seed
  srand(time(NULL));

  // Init the 'argv' parser
  cli::Parser parser(argc, argv);
  configure_parser(parser);
  parser.run_and_exit_if_error();

  // Get Player types from program options --white and --black.
  // An exception is trhown if player type is badly typed.
  try
    {
      PlayerType Whites = playerType(parser.get<std::string>("w"));
      PlayerType Blacks = playerType(parser.get<std::string>("b"));
      std::unique_ptr<ChessNeuNeu> chess = std::make_unique<ChessNeuNeu>(Whites, Blacks);
      chess->loop(new Board(*chess, chess->rules(), chess->m_players));
    }
  catch (std::string const& e)
    {
      std::cerr << "Fatal: " << e << std::endl;
      err = 1;
    }

  return err;
}
