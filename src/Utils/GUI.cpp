//=====================================================================
// ChessNeuNeu: Non serious chess engine for learning neural networks.
// Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of ChessNeuNeu.
//
// ChessNeuNeu is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#include "Utils/GUI.hpp"
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
