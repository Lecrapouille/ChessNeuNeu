#ifndef HUMAN_HPP
#  define HUMAN_HPP

#  include "Player.hpp"
#  include <mutex>
#  include <condition_variable>

class Human: public IPlayer
{
public:

  Human(const Rules &rules, const Color side);
  ~Human();
  virtual std::string play() override;
  virtual void abort() override;

  //! \brief Notification from the GUI.
  //! When the mouse released event occured
  //! The human has made a valid move and
  //! we need to get it.
  void notified(std::string const& move);

private:

  const Rules &m_rules;
  //using MuxGuard = std::lock_guard<std::mutex>;
  //mutable std::mutex m_lock;
  std::mutex m_mutex;
  std::condition_variable m_cond;
  std::string m_move;
};

#endif
