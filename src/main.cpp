#include "main.hpp"
#include "Players/Stockfish.hpp"
#include "Players/TSCP.hpp"
#include "Players/NeuNeu.hpp"
#include "Players/Human.hpp"
#include <cassert>

IPlayer *ChessGame::PlayerFactory(const PlayerType type, const Color side)
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
      return new Human(m_board, side);
    default:
      return nullptr;
    }
}

ChessGame::ChessGame(GUI* gui, cli::Parser& parser)
  : m_gui(gui), /*m_rules(Color::Black),*/ m_board(m_rules, gui->m_window)
{
  // Get Player types from program options --white and --black.
  // An exception is trhown if player type is badly typed.
  PlayerType white = playerType(parser.get<std::string>("w"));
  PlayerType black = playerType(parser.get<std::string>("b"));

  // Create two players
  m_players[Color::White].reset(PlayerFactory(white, Color::White));
  m_players[Color::Black].reset(PlayerFactory(black, Color::Black));

  // Debug
  std::cout
    << m_players[Color::White]->type()
    << " is playing as "
    << m_players[Color::White]->side()
    << std::endl
    << m_players[Color::Black]->type()
    << " is playing as "
    << m_players[Color::Black]->side()
    << std::endl;
  debug();
}

void ChessGame::debug()
{
  std::cout << m_rules.m_board << std::endl;
  std::cout << m_rules.m_side << " are playing";
  Status status = m_rules.generateValidMoves();
  if (status != Status::Playing)
    {
      std::cout << " and position is " << status;
    }
  std::cout << std::endl;
}

void ChessGame::draw(const float dt)
{
  m_board.draw();
}

void ChessGame::play(IPlayer& player, const Color color)
{
  std::string next_move;
  int max_errors = 0;

  if (m_rules.m_status != Status::Playing)
    return ;

  // FIXME
  if (PlayerType::HumanPlayer == player.type())
    return ;

  if (color != m_rules.m_side)
    return ;

  std::cout << player.type() << " is thinking" << std::endl;

  // "error" message instead of chess move can comes from of the following bugs (to be fixed):
  // -- Stockfish had no time for computing a move.
  // -- Rules class did not manage a rule (en passant, promotion, castle, mat)
  do {
    next_move = player.play();
    if (0 == next_move.compare("error"))
      {
        ++max_errors;
        std::cout << player.type() << " failed x" << max_errors << std::endl;
      }
    else
      {
        std::cout << "Move: " << next_move << std::endl;
        m_board.moveWithAnimation(next_move);
        m_rules.makeMove(next_move);
        return ;
      }
    if (max_errors > 7)
      m_rules.m_status = Status::InternalError;
  } while (max_errors <= 7);
}

void ChessGame::update(const float dt)
{
  play(*m_players[Color::White], Color::White);
  play(*m_players[Color::Black], Color::Black);

  if ((m_rules.m_status) && (m_previous_status != m_rules.m_status))
    {
      std::cout << "End of game: " << m_rules.m_status << std::endl;
    }
  m_previous_status = m_rules.m_status;
}

void ChessGame::handleInput()
{
  sf::Event event;
  m_board.mousePosition(sf::Mouse::getPosition(m_gui->m_window));
  while (m_gui->m_window.pollEvent(event))
    {
      switch (event.type)
        {
        case sf::Event::Closed:
          m_gui->m_window.close();
          break;

        case sf::Event::MouseButtonPressed:
          if (m_players[m_rules.m_side]->type() == PlayerType::HumanPlayer) // && (side == m_rules.side))
            {
              m_board.takeFigure();
            }
          break;

        case sf::Event::MouseButtonReleased:
          if (m_players[m_rules.m_side]->type() == PlayerType::HumanPlayer) // && (side == m_rules.side))
            {
              m_board.releaseFigure();
            }
          break;

        case sf::Event::KeyPressed:
          if (event.key.code == sf::Keyboard::BackSpace)
            {
              m_rules.moveBack();
              m_board.loadPosition(m_rules.m_board);
            }
          break;

        default:
          break;
        }
    }
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
  // Initialize random seed
  srand(time(NULL));

  // Init the 'argv' parser
  cli::Parser parser(argc, argv);
  configure_parser(parser);
  parser.run_and_exit_if_error();

  // Init and run the graphical user interface (GUI)
  GUI gui;
  gui.pushState(new ChessGame(&gui, parser));
  gui.mainLoop();

  return 0;
}
