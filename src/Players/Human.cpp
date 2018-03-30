#include "Human.hpp"

Human::Human(const Rules &rules, const Color side)
  : IPlayer(PlayerType::HumanPlayer, side), m_rules(rules)
{
}

Human::~Human()
{
}

void Human::abort()
{
  notified(IPlayer::error);
}

void Human::notified(std::string const& move)
{
  {
    std::unique_lock<std::mutex> mlock(m_mutex);
    m_move = move;
  }
  m_cond.notify_one();
}

std::string Human::play()
{
  std::unique_lock<std::mutex> mlock(m_mutex);

  m_cond.wait(mlock);
  std::string move(m_move);
  return move;
}
