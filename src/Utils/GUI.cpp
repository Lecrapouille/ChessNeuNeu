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

void Application::push(GUI& gui)
{
    m_guis.push(&gui);
    gui.activate();
}

void Application::pop()
{
    assert(!m_guis.empty());
    m_guis.top()->deactivate();
    m_guis.pop();
}

GUI* Application::peek()
{
    if (m_guis.empty())
        return nullptr;
    return m_guis.top();
}

void Application::loop(GUI& gui)
{
    GUI* g;

    // Push
    m_guis.push(&gui);
    gui.activate();

    // Infinite loop
    sf::Clock clock;

    do
    {
        float dt = clock.restart().asSeconds();
        g = peek();
        if (g != nullptr)
        {
            g->handleInput();
            g->update(dt);
            m_window.clear();
            g->draw(dt);
       }
    }
    while (g->isRunning());

    // Pop
    m_guis.top()->deactivate();
    m_guis.pop();
}
