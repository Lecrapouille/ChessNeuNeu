//=====================================================================
// ChessNeuNeu: Non serious chess engine for learning neural networks.
// Copyright 2018 -- 2022 Quentin Quadrat <lecrapouille@gmail.com>
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

#ifndef GUI_PROMOTION_HPP
#  define GUI_PROMOTION_HPP

#  include "Utils/GUI.hpp"
#  include "GUI/Resources.hpp"
#  include "Chess/Rules.hpp"

// *****************************************************************************
//! \brief GUI managing the pawn promotion. Display a board with the possible
//! pieces to promote. When the player selects one piece, this piece is used
//! for the game.
// *****************************************************************************
class Promotion: public GUI
{
public:

    //! \brief Constructor get references on game rules and the main window
    //! needed for drawing the GUI.
    Promotion(Application& application, Resources &resources, Color color);

    //! \brief Destructor. Release only GUI resources but not game rules.
    ~Promotion() = default;

    //! \brief Return the promoted figure.
    Piece promoted()
    {
        Piece promoted = m_promoted_figure;
        m_promoted_figure = NoPiece;
        return promoted;
    }

private:

    const Piece& getPiece(const sf::Vector2f& p) const;

    //! \brief Action on mouse button pressed event. Grab the piece (if
    //! present).
    Piece takeFigure();

    //! \brief Load chessboard and pieces textures.
    void loadTextures();

    //! \brief Place pieces on their position.
    void loadPosition();

    //! \brief Draw the chessboard and pieces.
    virtual void draw(const float dt) override;
    virtual void update(const float dt) override;
    virtual void handleInput() override;
    virtual bool isRunning() override;
    virtual void activate() override;
    virtual void deactivate() override;

private:

    //! \brief Reference on loaded resources (textures ...)
    Resources& m_resources;
    //! \brief Memorize the last moving figure.
    Piece m_promoted_figure = NoPiece;
    //! \brief
    chessboard m_board;
    //! \brief Memorize the mouse position when moving a figure.
    sf::Vector2f m_mouse;
};

#endif
