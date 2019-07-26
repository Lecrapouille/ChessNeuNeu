#ifndef GUI_CONFIG_HPP
#  define GUI_CONFIG_HPP

#  include <SFML/Graphics.hpp>

// ***********************************************************************************************
//! \brief Define dimensions for the GUI
// ***********************************************************************************************
namespace conf
{
  namespace dim
  {
    //! \brief The texture dimension for a piece (square).
    int const figure = 56;
    //! \brief The texture dimension of the chessboard border (offset).
    int const border = 28;
    //! \brief The texture dimension of the chessboard without its border (8 figures).
    int const board = 8 * conf::dim::figure;
  }

  namespace res
  {
    //! \brief chessboard and figure styles.
    std::string const style("style01");
    std::string const path("data/" + conf::res::style + "/");
  }
}

#endif
