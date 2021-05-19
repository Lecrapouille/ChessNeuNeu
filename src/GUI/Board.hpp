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

#ifndef GUI_BOARD_HPP
#  define GUI_BOARD_HPP

#  include "Utils/GUI.hpp"
#  include "Players/Human.hpp"
#  include "GUI/Resources.hpp"
#  include "Chess/Rules.hpp"
#  include <thread>
#  include <atomic>

class Promotion;

// *****************************************************************************
//! \brief Class displaying a chess board, its figures (piece sprite) and let to
//! the player moving pieces. This code has been inspired by the Youtube video
//! "Let's make 16 games in C++: Chess" by FamTrinli but has been adapted for my
//! own purpose.
// *****************************************************************************
class Board: public GUI
{
public:

    //! \brief Constructor.
    Board(Application& application, Rules &rules, Resources &resources,
          std::shared_ptr<IPlayer> players[2]);

    //! \brief Destructor
    ~Board();

private:

    //! \brief infinite loop launched by the thread.
    //!
    //! Respond to human interaction: mouse (grab/ungrab figure), keyboard 'ESC
    //! for leaving the application), rendering (refresh the chessboard and its
    //! figures), react on the chess player move and animate the figure
    //! movement.
    //!
    //! Even if the game is ended (win, draw ...), the infinite loop is
    //! maintained. The loop will be halt only when the user asked to close the
    //! window.
    void play();

    //! \brief Place figures on their initial position.
    void loadPosition(chessboard const& board);

    //! \brief Action on mouse button pressed event. Grab the figure (if
    //! present).
    //! \return true if the figure is grabbed.
    bool takeFigure(sf::Vector2f const& mouse);

    //! \brief Action on mouse button pressed event. Ubgrab the piece (if
    //! grabbed).
    //! \return true if the figure is ungrabbed.
    bool releaseFigure(sf::Vector2f const& mouse);

    //! \brief Inherit from GUI class. Draw the chessboard and pieces.
    virtual void draw(const float dt) override;

    //! \brief Inherit from GUI class. Update GUI.
    virtual void update(const float dt) override;

    //! \brief Inherit from GUI class. Manage mouse and keyboard events.
    virtual void handleInput() override;

    //! \brief Inherit from GUI class. Return true if GUI is alive.
    virtual bool isRunning() override;

    //! \brief Called when the GUI has been enabled.
    virtual void activate() override {}

    //! \brief Called when the GUI has been disabled.
    virtual void deactivate() override {}

    //! \brief Release the taken piece.
    void ungrabFigure();

    //! \brief Is the player holding a piece for making its move ?
    bool grabbedFigure() const;

    //! \brief Get the piece under the mouse cursor.
    Piece const& getPiece(sf::Vector2f const& mouse) const;

    //! \brief Get the chessboard square under the mouse cursor.
    Square getSquare(sf::Vector2f const& mouse) const;

    //! \brief Animate the piece move.
    //! \param move the newly played move in format like "e2e4".
    //! \note: the move shall be valid !
    void animate(const std::string& move);

    //! \brief Return coordinate of the chessboard square.
    //! \param a column 'a' .. 'h' of the chessboard square.
    //! \param b line '1' .. '8' of the chessboard square.
    sf::Vector2f toCoord(const char a, const char b) const;

    //! \brief SIGINT signal handler
    static void sigintHandler(int signo);

private:

    //! \brief Reference on loaded resources (textures ...)
    Resources         &m_resources;
    //! \brief Reference on the game rules.
    Rules             &m_rules;
    //! \brief Memorize the taken figure by the user.
    //! Refer to m_resources.figures[x].
    uint8_t           m_grabbed;
    //! \brief the start square of a piece movement
    uint8_t           m_from = Square::OOB;
    //! \brief the arrival square of a piece movement
    uint8_t           m_to = Square::OOB;
    //! \brief Memorize the mouse position when moving a figure.
    sf::Vector2f       m_mouse;
    bool               m_updated = true;
    std::string        m_move;

    std::shared_ptr<IPlayer> m_players[2];
    std::unique_ptr<Promotion> m_gui_promotion[2];
    std::string        m_opponent_move;
    std::atomic_bool   m_running_thread{true};
    std::atomic_bool   m_animating{false};
    std::thread        m_thread;
    using MuxGuard = std::lock_guard<std::mutex>;
    mutable std::mutex m_lock;
};

#endif
