#include "Stockfish.hpp"

Stockfish::Stockfish(const Rules &rules, const Color side, std::string const& fen)
  : IPC("stockfish"),
    IPlayer(PlayerType::StockfishIA, side),
    m_fen(fen),
    m_rules(rules)
{
  std::cout << read() << std::endl;
}

Stockfish::~Stockfish()
{
}

void Stockfish::abort()
{
  m_aborting = true;
}

std::string Stockfish::play()
{
  // Command to send to Stockfish
  std::string command("position ");

  if (m_fen.empty())
    {
      // Initial chessboard
      command += "startpos";
    }
  else
    {
      // Load the chessboard with the Forsyth–Edwards notation
      command += "fen ";
      command += m_fen;
    }

  // Add the list of moves
  command += " moves ";
  command += m_rules.m_moved;
  command += "\ngo\n";

  // Send the command to Stockfish
  write(command);

  // FIXME: Give time to Stockfish
  usleep(100000);

  // Get and parse the Stockfish answer
  std::string answer = read();
  int n = answer.find("bestmove");
  if (n != -1)
    {
      std::string move = answer.substr(n + 9, 5);

      if (m_aborting)
        return IPlayer::error;
      return move;
    }

  // FIXME: Retry
  return IPlayer::error;
}

void Stockfish::debug()
{
  write("d\n");
  std::cout << read() << std::endl;
}
