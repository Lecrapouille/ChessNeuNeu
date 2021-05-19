#ifndef GUI_RESOURCES_HPP
#define GUI_RESOURCES_HPP

#  include "Config.hpp"
#  include "Chess/Pieces.hpp"

// ***********************************************************************************************
//! \brief Define GUI resources (textures ...). This class is shared for the chessboard and the
//! pawn promotion selection.
//!
//! Textures are picture loaded in the CPU memory from files disk (jpeg, png ...). They are then
//! transfered in the GPU memory. Important: Variable storing textures shall not be released while
//! the program is not ended, else white squares appear.
//!
//! A sprite is a textured rectangle knowing its position on the screen.
// ***********************************************************************************************
class Resources
{
public:

    //! \brief Load resources
    Resources(std::string const& path_figures, std::string const& path_board)
    {
        bool res1 = textures[0].loadFromFile(conf::res::path + path_figures);
        bool res2 = textures[1].loadFromFile(conf::res::path + path_board);

        if (!res1 || !res2)
            throw std::string("Failed loading textures");

        board.setTexture(textures[1]);
        for (uint8_t i = 0u; i < NbPieces; ++i)
        {
            figures[i].setTexture(textures[0]);
        }
    }

    //! \brief Chessboard and piece textures. Piece texture is an atlas texture.
    sf::Texture        textures[2];
    //! \brief A figure is a chess pieces texture knowing its position.
    sf::Sprite         figures[NbPieces];
    //! \brief Chessboard sprite.
    sf::Sprite         board;
};

#endif
