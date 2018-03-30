#include "GUI/GUI.hpp"
#include <iostream>

Application::Application()
{
  m_window.create(sf::VideoMode(504, 504), "ChessNeuNeu");
  //m_window.setFramerateLimit(10);
}

Application::~Application()
{
  while (!m_guis.empty())
    {
      pop();
    }
}

void Application::push(GUI* gui)
{
  assert(nullptr != gui);
  m_guis.push(gui);
}

void Application::pop()
{
  assert(!m_guis.empty());
  GUI* gui = m_guis.top();
  assert(nullptr != gui);
  delete gui;
  m_guis.pop();
}

GUI* Application::peek()
{
  if (m_guis.empty())
    return nullptr;
  return m_guis.top();
}

void Application::loop(GUI* gui)
{
  // Push
  assert(nullptr != gui);
  m_guis.push(gui);

  // Infinite loop
  sf::Clock clock;
  while (gui->running())
    {
      float dt = clock.restart().asSeconds();

      peek()->handleInput();
      peek()->update(dt);
      m_window.clear();
      peek()->draw(dt);

    }

  // Pop
  delete m_guis.top();
  m_guis.pop();
}
