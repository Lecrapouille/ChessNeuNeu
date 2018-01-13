#ifndef STATES_HPP
#  define STATES_HPP

#  include <string>

// ***********************************************************************************************
//! \brief Structure storing the piece position on the board and all movements.
//! This structure can be used by other classes.
// ***********************************************************************************************
struct States
{
  States()
    : board{ { -1,-2,-3,-4,-5,-3,-2,-1 },
             { -6,-6,-6,-6,-6,-6,-6,-6 },
             {  0, 0, 0, 0, 0, 0, 0, 0 },
             {  0, 0, 0, 0, 0, 0, 0, 0 },
             {  0, 0, 0, 0, 0, 0, 0, 0 },
             {  0, 0, 0, 0, 0, 0, 0, 0 },
             {  6, 6, 6, 6, 6, 6, 6, 6 },
             {  1, 2, 3, 4, 5, 3, 2, 1 } }
  {
  }

  std::string        moves;
  int                board[8][8];
};

#endif
