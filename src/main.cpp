#include "main.hpp"
#include <algorithm>
#include <cassert>

static void configure_parser(cli::Parser& parser)
{
  parser.set_optional<std::string>("w", "white", "Human", "Define the white player: Human | Stockfish | NeuNeu");
  parser.set_optional<std::string>("b", "black", "Stockfish", "Define the black player: Human | Stockfish | NeuNeu");
}

IPlayer *NeuNeuGUI::PlayerFactory(const PlayerType type, const Color side)
{
  switch (type)
    {
    case PlayerType::StockfishIA:
      return new Stockfish(m_rules, side);
    case PlayerType::TcspIA:
      return new Tcsp(m_rules, side);
    case PlayerType::NeuNeuIA:
      return new NeuNeu(m_rules, side);
    case PlayerType::HumanPlayer:
      return new Human(m_board, side);
    default:
      return nullptr;
    }
}

static PlayerType toPlayerType(const std::string& player)
{
  std::string name(player);
  std::transform(name.begin(), name.end(), name.begin(), ::tolower);
  if (name == "stockfish") return PlayerType::StockfishIA;
  if (name == "neuneu") return PlayerType::NeuNeuIA;
  if (name == "human") return PlayerType::HumanPlayer;
  if (name == "tcsp") return PlayerType::TcspIA;

  std::cerr << "Unknonw PlayerType: " << player << std::endl;
  exit(1);
}

NeuNeuGUI::NeuNeuGUI(GUI* gui, cli::Parser& parser)
  : GUIState(), m_gui(gui), /*m_rules(Color::Black),*/ m_board(m_rules, gui->m_window)
{
  m_players[Color::White] = PlayerFactory(toPlayerType(parser.get<std::string>("w")), Color::White);
  m_players[Color::Black] = PlayerFactory(toPlayerType(parser.get<std::string>("b")), Color::Black);

  for (uint8_t i = 0; i < 2u; ++i)
   {
     std::cout << m_players[i]->type() << " is playing as " << m_players[i]->side() << std::endl;
   }

  std::cout << m_rules.m_current_position << std::endl;
  std::cout << m_rules.m_side << " are playing";
  Status status = m_rules.generateValidMoves();
  if (status != Status::Playing)
    {
      std::cout << " and position is " << status;
    }
  std::cout << std::endl;
}

void NeuNeuGUI::draw(const float dt)
{
  m_board.draw();
}

void NeuNeuGUI::play(IPlayer *player, const Color color)
{
  std::string next_move;
  int max_errors = 0;

  if (m_rules.m_status != Status::Playing)
    return ;

  if ((player->type() != PlayerType::HumanPlayer) && (color == m_rules.m_side))
    {
      std::cout << player->type() << " is thinking" << std::endl;

      // "error" message instead of chess move can comes from of the following bugs (to be fixed):
      // -- Stockfish had no time for computing a move.
      // -- Rules class did not manage a rule (en passant, promotion, castle, mat)
      do {
        next_move = player->nextMove();
        if (0 == next_move.compare("error"))
          {
            ++max_errors;
            std::cout << player->type() << " failed x" << max_errors << std::endl;
          }
        else
          {
            m_board.oppentMove(next_move);
            return ;
          }
        if (max_errors > 7)
          m_rules.m_status = Status::InternalError;
      } while (max_errors <= 7);
    }
}

void NeuNeuGUI::update(const float dt)
{
  play(m_players[Color::White], Color::White);
  play(m_players[Color::Black], Color::Black);

  /*if ((g_end_of_game) && (g_previous_end_of_game != g_end_of_game))
    {
      if (m_rules.m_status == Status::Playing)
        std::cout << "End of game because NeuNeu got internal failures" << std::endl;
      else
        std::cout << "End of game: " << m_rules.m_status << std::endl;
    }
    g_previous_end_of_game = g_end_of_game;*/

  if ((m_rules.m_status) && (m_previous_status  != m_rules.m_status))
    {
      std::cout << "End of game: " << m_rules.m_status << std::endl;
    }
  m_previous_status = m_rules.m_status;
}

void NeuNeuGUI::handleInput()
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
              m_board.moveBack();
            }
          break;

        default:
          break;
        }
    }
}

int main(int argc, char** argv)
{
  // Initialize random seed
  srand (time(NULL));

  // Init the 'argv' parser
  cli::Parser parser(argc, argv);
  configure_parser(parser);
  parser.run_and_exit_if_error();

  // Init and run the graphical user interface (GUI)
  GUI gui;
  gui.pushState(new NeuNeuGUI(&gui, parser));
  gui.mainLoop();

  return 0;
}
