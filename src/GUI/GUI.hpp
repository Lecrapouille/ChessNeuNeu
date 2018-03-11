#ifndef GUI_HPP
#  define GUI_HPP

#  include <SFML/Graphics.hpp>
#  include <SFML/System.hpp>
#  include <stack>

//! \file Inspired by
//! https://www.binpress.com/tutorial/creating-a-city-building-game-with-sfml/137

class GUIState;

// ***********************************************************************************************
//! \brief Manage a stack of GUIState class
// ***********************************************************************************************
class GUI
{
public:

  GUI();
  ~GUI();

  void pushState(GUIState* state);
  void popState();
  void changeState(GUIState* state);
  GUIState* peekState();
  void mainLoop();

  sf::RenderWindow m_window;

private:

  std::stack<GUIState*> m_states_stack;
};

// ***********************************************************************************************
//! \brief Interface class for drawing a windows and handling to mouse and keyboard events.
// ***********************************************************************************************
class GUIState
{
  friend class GUI;

public:

  GUIState() {}
  virtual ~GUIState() {}

private:

  virtual void draw(const float dt) = 0;
  virtual void update(const float dt) = 0;
  virtual void handleInput() = 0;

  GUI* m_gui;
};

#endif
