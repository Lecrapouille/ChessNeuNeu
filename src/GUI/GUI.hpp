#ifndef GUI_HPP
#  define GUI_HPP

#  include <SFML/Graphics.hpp>
#  include <SFML/System.hpp>
#  include <stack>

//! \file Inspired by
//! https://www.binpress.com/tutorial/creating-a-city-building-game-with-sfml/137

class GUI;

class GUIState
{
public:

  GUIState() {}
  virtual ~GUIState() {}
  virtual void draw(const float dt) = 0;
  virtual void update(const float dt) = 0;
  virtual void handleInput() = 0;

  GUI* m_gui;
};

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
  std::stack<GUIState*> m_states_stack;
};

#endif
