#ifndef GUI_DIMENSION_HPP
#  define GUI_DIMENSION_HPP

#  include <SFML/Graphics.hpp>

// ***********************************************************************************************
//! \brief Define constants for the board
// ***********************************************************************************************
namespace config
{
  namespace dim
  {
    //! \brief the dimension of the squared texture for each piece.
    const int          figure = 56;
    //! \brief The boder dimension (x, y) of the chessboard.
    const sf::Vector2f border{28, 28};
    //! \brief Chessboard dimension
    const sf::Vector2f board
    {
      8.0f * 2.0f * config::dim::border.x,
      8.0f * 2.0f * config::dim::border.y
    };
  }
};

#endif
