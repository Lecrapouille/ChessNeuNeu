//=====================================================================
// ChessNeuNeu: Non serious chess engine for learning neural networks.
// Copyright 2018 -- 2022-2022 Quentin Quadrat <lecrapouille@gmail.com>
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

#ifndef GUI_RESOURCES_HPP
#  define GUI_RESOURCES_HPP

#  include "Config.hpp"
#  include "Chess/Pieces.hpp"
#  include <sys/stat.h>

#  ifdef __APPLE__
#    include <CoreFoundation/CFBundle.h>
#  endif

// *****************************************************************************
//! \brief Define SFML resources (textures, sprited ...). This class is shared
//! by GUIs: chessboard and the pawn promotion selection.
//!
//! Textures are picture loaded in the CPU memory from files disk (jpeg,
//! png ...). They are then transfered in the GPU memory. Important: Variable
//! storing textures shall not be released while the program is not ended, else
//! white squares will appeared.
//!
//! Sprites are textured rectangles knowing their positions on the screen.
// *****************************************************************************
class Resources
{
public:

    //--------------------------------------------------------------------------
    //! \brief Load resources from the OS
    //--------------------------------------------------------------------------
    Resources(std::string const& path_figures, std::string const& path_board)
    {
        bool res1 = textures[0].loadFromFile(
            data_path(conf::res::style + "/" + path_figures));
        bool res2 = textures[1].loadFromFile(
            data_path(conf::res::style + "/" + path_board));

        if (!res1 || !res2)
            throw std::string("Failed loading textures");

        board.setTexture(textures[1]);
        for (uint8_t i = 0u; i < NbPieces; ++i)
        {
            figures[i].setTexture(textures[0]);
        }
    }
private:

    //--------------------------------------------------------------------------
    //! \brief Return the data folder.
    //--------------------------------------------------------------------------
    std::string data_path(std::string const& file) const
    {
        struct stat exists; // folder exists ?
        std::string path;

#  ifdef __APPLE__

        // Return the resources folder inside MacOS bundle application
        CFURLRef resourceURL = CFBundleCopyResourcesDirectoryURL(CFBundleGetMainBundle());
        char resourcePath[PATH_MAX];
        if (CFURLGetFileSystemRepresentation(resourceURL, true,
                                             reinterpret_cast<UInt8 *>(resourcePath),
                                             PATH_MAX))
        {
            if (resourceURL != NULL)
            {
                CFRelease(resourceURL);
            }

            path = std::string(resourcePath) + "/" + file;
            if (stat(path.c_str(), &exists) == 0)
            {
                return path;
            }
        }

#endif

#ifdef DATADIR
        path = std::string(DATADIR) + "/" + file;
        if (stat(path.c_str(), &exists) == 0)
        {
            return path;
        }
#endif

        path = "data/" + file;
        if (stat(path.c_str(), &exists) == 0)
        {
            return path;
        }

        return file;
    }

public:

    //! \brief Chessboard and piece textures. Piece texture is an atlas texture.
    sf::Texture        textures[2];
    //! \brief A figure is a chess pieces texture knowing its position.
    sf::Sprite         figures[NbPieces];
    //! \brief Chessboard sprite.
    sf::Sprite         board;
};

#endif
