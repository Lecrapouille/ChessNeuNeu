#ifndef CHESS_BOARD_HPP
#  define CHESS_BOARD_HPP

#  include "Chess/Pieces.hpp"
#  include <array>
#  include <string>

//! \brief Number of squares in a chessboard.
constexpr uint8_t NbSquares = 64u;

//! \brief A chessboard is a 8x8 matrix of Pieces.
//! The 8x8 matrix is stored as a continuous block
//! of 64 elements.
using chessboard = std::array<Piece, NbSquares>;

//! \brief Access to chessboard rows as a 8x8 matrix.
//! Mathematic formulae: x / 8.
#  define ROW(x)   (x >> 3)

//! \brief Access to chessboard columns  as a 8x8 matrix.
//! Mathematic formulae: x modulo 8.
#  define COL(x)   (x & 7)

//! \brief Give an number for all of the chessboard squares.
//! Note: 'sq' prefix is necessary because of conflicts with
//! SFML names.
enum Square
  {
    sqA8, sqB8, sqC8, sqD8, sqE8, sqF8, sqG8, sqH8,
    sqA7, sqB7, sqC7, sqD7, sqE7, sqF7, sqG7, sqH7,
    sqA6, sqB6, sqC6, sqD6, sqE6, sqF6, sqG6, sqH6,
    sqA5, sqB5, sqC5, sqD5, sqE5, sqF5, sqG5, sqH5,
    sqA4, sqB4, sqC4, sqD4, sqE4, sqF4, sqG4, sqH4,
    sqA3, sqB3, sqC3, sqD3, sqE3, sqF3, sqG3, sqH3,
    sqA2, sqB2, sqC2, sqD2, sqE2, sqF2, sqG2, sqH2,
    sqA1, sqB1, sqC1, sqD1, sqE1, sqF1, sqG1, sqH1,
    // Out of Bound
    OOB
  };

//! \brief Store square names. Mainly used for creating
//! move notes.
constexpr std::array<const char[3], NbSquares> c_square_names =
  {{
      "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
      "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
      "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
      "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
      "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
      "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
      "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
      "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
    }};

constexpr Square toSquare(const char* square)
{
  int val = ('8' - square[1]) * 8 + (square[0] - 'a');
  assert(val < NbSquares);
  return static_cast<Square>(val);
}

inline std::string toStrMove(const uint8_t from, const uint8_t to)
{
  return std::string(c_square_names[from]) + std::string(c_square_names[to]);
}

namespace Chessboard
{

  //! \brief Define the initial chessboard position. You can use the operator= for copy it.
  constexpr chessboard Init =
    {{
        BlackRook, BlackKnight, BlackBishop, BlackQueen, BlackKing, BlackBishop, BlackKnight, BlackRook,
        BlackPawn, BlackPawn,   BlackPawn,   BlackPawn,  BlackPawn, BlackPawn,   BlackPawn,   BlackPawn,
        NoPiece,   NoPiece,     NoPiece,     NoPiece,    NoPiece,   NoPiece,     NoPiece,     NoPiece,
        NoPiece,   NoPiece,     NoPiece,     NoPiece,    NoPiece,   NoPiece,     NoPiece,     NoPiece,
        NoPiece,   NoPiece,     NoPiece,     NoPiece,    NoPiece,   NoPiece,     NoPiece,     NoPiece,
        NoPiece,   NoPiece,     NoPiece,     NoPiece,    NoPiece,   NoPiece,     NoPiece,     NoPiece,
        WhitePawn, WhitePawn,   WhitePawn,   WhitePawn,  WhitePawn, WhitePawn,   WhitePawn,   WhitePawn,
        WhiteRook, WhiteKnight, WhiteBishop, WhiteQueen, WhiteKing, WhiteBishop, WhiteKnight, WhiteRook,
      }};

  //! \brief Define a chessboard with no piece (and no kings). You can use the operator= for copy it.
  constexpr chessboard Empty =
    {{
        NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece,
        NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece,
        NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece,
        NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece,
        NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece,
        NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece,
        NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece,
        NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece, NoPiece,
      }};
}

//! \brief Pretty print a chessboard in console
std::ostream& operator<<(std::ostream& os, chessboard& position);

#endif
