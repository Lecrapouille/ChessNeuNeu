#ifndef PIECES_HPP
#  define PIECES_HPP

#  include <iostream>
#  include <array>

//! \brief Define Side color.
enum Color { Black, White };

constexpr Color opposite(const Color c) { return Color::White == c ? Color::Black : Color::White; }

//! \brief Define the different type of piece on the chessboard.
enum PieceType { Empty, Rook, Knight, Bishop, Queen, King, Pawn, NotUsed };

//! \brief The type of a piece is not the only important informations: we also need the color.
//! and if a piece have moved (for castle for example). Some informations are stored to avoid
//! searching them many times.
struct Piece
{
  unsigned int color : 1;    // Bit 7: store Color enum
  unsigned int _attacked: 1; // Bit 6: if an opponent piece attacks this piece (check for the King)
  unsigned int _pined: 1;    // Bit 5: if an opponent piece pins the King
  unsigned int moved : 1;    // Bit 4: piece has moved
  unsigned int type : 4;     // Bit 3-0: store PieceType enum
};

//! \brief Empty square when there is no piece on the square.
//! FIXME the fake White color creates problem
constexpr Piece EmptySquare = { Color::White, 0, 0, 0, PieceType::Empty };

constexpr Piece WhitePawn = { Color::White, 0, 0, 0, PieceType::Pawn };
constexpr Piece WhiteKnight = { Color::White, 0, 0, 0, PieceType::Knight };
constexpr Piece WhiteBishop = { Color::White, 0, 0, 0, PieceType::Bishop };
constexpr Piece WhiteRook = { Color::White, 0, 0, 0, PieceType::Rook };
constexpr Piece WhiteQueen = { Color::White, 0, 0, 0, PieceType::Queen };
constexpr Piece WhiteKing = { Color::White, 0, 0, 0, PieceType::King };

constexpr Piece BlackPawn = { Color::Black, 0, 0, 0, PieceType::Pawn };
constexpr Piece BlackKnight = { Color::Black, 0, 0, 0, PieceType::Knight };
constexpr Piece BlackBishop = { Color::Black, 0, 0, 0, PieceType::Bishop };
constexpr Piece BlackRook = { Color::Black, 0, 0, 0, PieceType::Rook };
constexpr Piece BlackQueen = { Color::Black, 0, 0, 0, PieceType::Queen };
constexpr Piece BlackKing = { Color::Black, 0, 0, 0, PieceType::King };

using chessboard = std::array<Piece, 64u>;

//! \brief Define the initial chessboard position. You can use the operator= for copy it.
constexpr chessboard c_init_board =
  {{
      BlackRook,   BlackKnight,  BlackBishop,  BlackQueen,  BlackKing,   BlackBishop,  BlackKnight,  BlackRook,
      BlackPawn,   BlackPawn,    BlackPawn,    BlackPawn,   BlackPawn,   BlackPawn,    BlackPawn,    BlackPawn,
      EmptySquare, EmptySquare,  EmptySquare,  EmptySquare, EmptySquare, EmptySquare,  EmptySquare,  EmptySquare,
      EmptySquare, EmptySquare,  EmptySquare,  EmptySquare, EmptySquare, EmptySquare,  EmptySquare,  EmptySquare,
      EmptySquare, EmptySquare,  EmptySquare,  EmptySquare, EmptySquare, EmptySquare,  EmptySquare,  EmptySquare,
      EmptySquare, EmptySquare,  EmptySquare,  EmptySquare, EmptySquare, EmptySquare,  EmptySquare,  EmptySquare,
      WhitePawn,   WhitePawn,    WhitePawn,    WhitePawn,   WhitePawn,   WhitePawn,    WhitePawn,    WhitePawn,
      WhiteRook,   WhiteKnight,  WhiteBishop,  WhiteQueen,  WhiteKing,   WhiteBishop,  WhiteKnight,  WhiteRook,
    }};

//! \brief Define a chessboard with no piece (and no kings). You can use the operator= for copy it.
constexpr chessboard c_empty_board =
  {{
      EmptySquare, EmptySquare, EmptySquare, EmptySquare, EmptySquare, EmptySquare, EmptySquare, EmptySquare,
      EmptySquare, EmptySquare, EmptySquare, EmptySquare, EmptySquare, EmptySquare, EmptySquare, EmptySquare,
      EmptySquare, EmptySquare, EmptySquare, EmptySquare, EmptySquare, EmptySquare, EmptySquare, EmptySquare,
      EmptySquare, EmptySquare, EmptySquare, EmptySquare, EmptySquare, EmptySquare, EmptySquare, EmptySquare,
      EmptySquare, EmptySquare, EmptySquare, EmptySquare, EmptySquare, EmptySquare, EmptySquare, EmptySquare,
      EmptySquare, EmptySquare, EmptySquare, EmptySquare, EmptySquare, EmptySquare, EmptySquare, EmptySquare,
      EmptySquare, EmptySquare, EmptySquare, EmptySquare, EmptySquare, EmptySquare, EmptySquare, EmptySquare,
      EmptySquare, EmptySquare, EmptySquare, EmptySquare, EmptySquare, EmptySquare, EmptySquare, EmptySquare,
    }};

//! \brief Print the type of piece.
std::ostream& operator<<(std::ostream& os, const PieceType& p);
//! \brief Print the type of piece.
std::ostream& operator<<(std::ostream& os, const Piece& p);
//! \brief Print the piece color.
std::ostream& operator<<(std::ostream& os, const Color& c);

#endif
