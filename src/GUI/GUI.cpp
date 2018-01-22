#include "GUI.hpp"

GUI::GUI()
{
  m_window.create(sf::VideoMode(504, 504), "NeuNeuChess");
  //m_window.setFramerateLimit(10);
}

GUI::~GUI()
{
  while (!m_states_stack.empty())
    popState();
}

void GUI::pushState(GUIState* state)
{
  m_states_stack.push(state);
}

void GUI::popState()
{
  delete m_states_stack.top();
  m_states_stack.pop();
}

void GUI::changeState(GUIState* state)
{
  if (!m_states_stack.empty())
    popState();
  pushState(state);
}

GUIState* GUI::peekState()
{
  if (m_states_stack.empty())
    return nullptr;
  return m_states_stack.top();
}

void GUI::mainLoop()
{
  sf::Clock clock;

  while (m_window.isOpen())
    {
      sf::Time elapsed = clock.restart();
      float dt = elapsed.asSeconds();

      if (peekState() == nullptr)
        continue;
      peekState()->handleInput();
      peekState()->update(dt);
      m_window.clear();
      peekState()->draw(dt);
      //m_window.display();
    }
}
