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

#ifndef GUI_GUI_HPP
#  define GUI_GUI_HPP

#  include <SFML/Graphics.hpp>
#  include <SFML/System.hpp>
#  include <stack>
#  include <cassert>

//! \file Inspired by
//! https://www.binpress.com/tutorial/creating-a-city-building-game-with-sfml/137

class GUI;

// ***********************************************************************************************
//! \brief Manage a stack of GUI class
// ***********************************************************************************************
class Application
{
  friend class GUI;

public:

  Application();
  ~Application();

  void loop(GUI* gui);
  void run(GUI* state);
  void push(GUI* state);
  void pop();
  GUI* peek();

private:

  std::stack<GUI*> m_guis; // FIXME: use unique_ptr and move
  sf::RenderWindow m_window;
};

// ***********************************************************************************************
//! \brief Interface class for drawing a windows and handling to mouse and keyboard events.
// ***********************************************************************************************
class GUI
{
  friend class Application;

public:

  GUI(Application& application)
    : m_application(application)
  {
  }

  virtual ~GUI() {}

  sf::RenderWindow& window()
  {
    return m_application.m_window;
  }

private:

  virtual bool running() = 0;
  virtual void draw(const float dt) = 0;
  virtual void update(const float dt) = 0;
  virtual void handleInput() = 0;

public:

  Application& m_application;
};

#endif
