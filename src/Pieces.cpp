#include "Pieces.hpp"
#include <array>

//! \brief Store piece names (white side). Used for drawing in the console a chessboard.
static const std::array<char, 8> c_white_piece_char =
  {{
      [PieceType::Empty] = ' ',
      [PieceType::Rook] = 'R',
      [PieceType::Knight] = 'N',
      [PieceType::Bishop] = 'B',
      [PieceType::Queen] = 'Q',
      [PieceType::King] = 'K',
      [PieceType::Pawn] = 'P',
      [PieceType::NotUsed] = ' ',
    }};

//! \brief Store piece names (black side). Used for drawing in the console a chessboard.
static const std::array<char, 8> c_black_piece_char =
  {{
      [PieceType::Empty] = ' ',
      [PieceType::Rook] = 'r',
      [PieceType::Knight] = 'n',
      [PieceType::Bishop] = 'b',
      [PieceType::Queen] = 'q',
      [PieceType::King] = 'k',
      [PieceType::Pawn] = 'p',
      [PieceType::NotUsed] = ' ',
    }};

//! \brief Print the piece color.
std::ostream& operator<<(std::ostream& os, const Color& c)
{
  os << ((Color::White == c) ? "White" : "Black");
  return os;
}

//! \brief Print the type of piece.
std::ostream& operator<<(std::ostream& os, const PieceType& p)
{
  os << c_white_piece_char[p];
  return os;
}

//! \brief Print the type of piece.
// TODO: print other info (castle, moved, ...)
std::ostream& operator<<(std::ostream& os, const Piece& p)
{
  if (p.color == Color::White)
    {
      os << c_white_piece_char[p.type];
    }
  else
    {
      os << c_black_piece_char[p.type];
    }
  return os;
}
