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
        const int figure = 56;
        //! \brief The texture dimension of the chessboard border (offset).
        const int border = 28;
        //! \brief The texture dimension of the chessboard without its border (8 figures).
        const int board = 8 * conf::dim::figure;
    }

    namespace res
    {
        //! \brief chessboard and figure styles.
        const std::string style("style01");
        const std::string path("data/" + conf::res::style + "/");
    }
}

#endif
