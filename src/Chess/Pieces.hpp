//=====================================================================
// ChessNeuNeu: Non serious chess engine for learning neural networks.
// Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
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

#ifndef CHESS_PIECES_HPP
#  define CHESS_PIECES_HPP

#  include <iostream>
#  include <map>
#  include <cassert>

//! \brief Max number of pieces in a chessboard.
constexpr uint8_t NbPieces = 32u;

//! \brief Define Side color.
enum Color { Black, White };

//! \brief Return the opposite color.
constexpr Color opposite(const Color c) { return Color::White == c ? Color::Black : Color::White; }

//! \brief Define the different type of piece on the chessboard.
//! Empty: for empty square.
//! NotUsed: for completing enum to 2^4 of the bit field of Piece.type.
//! WPawn and BPawn (White and black pawn) used for matching with figures of the neural network.
enum PieceType { Empty, Rook, Knight, Bishop, Queen, King, Pawn, NotUsed, WPawn = Pawn, BPawn = NotUsed };

//! \brief The type of a piece is not the only important informations: we also need the color.
//! and if a piece have moved (for castle for example). Some informations are stored to avoid
//! searching them many times.
struct Piece
{
    unsigned int color : 2;    // Bit 7-6: store Color enum
    unsigned int slide : 1;    // Bit 5  : can do more than one relative movement
    unsigned int moved : 1;    // Bit 4  : piece has moved
    unsigned int type  : 4;    // Bit 3-0: store PieceType enum
};

//! \brief Piece comparator. We only compare color and type of piece.
//! Other informations are ignored.
inline bool operator==(const Piece& lhs, const Piece& rhs)
{
    return (lhs.color == rhs.color) && (lhs.type == rhs.type);
}

//! \brief Piece comparator. We only compare color and type of piece.
//! Other informations are ignored.
inline bool operator!=(const Piece& lhs, const Piece& rhs)
{
    return !(lhs == rhs);
}

//! \brief Empty square when there is no piece on the square.
constexpr Piece NoPiece     = { Color::White + 1, 0, 0, PieceType::Empty };

constexpr Piece WhitePawn   = { Color::White,   0, 0, PieceType::Pawn };
constexpr Piece WhiteKnight = { Color::White,   0, 0, PieceType::Knight };
constexpr Piece WhiteBishop = { Color::White,   1, 0, PieceType::Bishop };
constexpr Piece WhiteRook   = { Color::White,   1, 0, PieceType::Rook };
constexpr Piece WhiteQueen  = { Color::White,   1, 0, PieceType::Queen };
constexpr Piece WhiteKing   = { Color::White,   0, 0, PieceType::King };

constexpr Piece BlackPawn   = { Color::Black,   0, 0, PieceType::Pawn };
constexpr Piece BlackKnight = { Color::Black,   0, 0, PieceType::Knight };
constexpr Piece BlackBishop = { Color::Black,   1, 0, PieceType::Bishop };
constexpr Piece BlackRook   = { Color::Black,   1, 0, PieceType::Rook };
constexpr Piece BlackQueen  = { Color::Black,   1, 0, PieceType::Queen };
constexpr Piece BlackKing   = { Color::Black,   0, 0, PieceType::King };

//! \brief Convert an ascii char to a Piece class.
//! \param c shall be one char from "prbnkqPRBNKQ"
//! \return the Piece if c is valid else return NoPiece.
Piece char2Piece(const char c);
char piece2char(const PieceType p);
char piece2char(const Piece p);
//! \brief Print the type of piece.
std::ostream& operator<<(std::ostream& os, const PieceType& p);
//! \brief Print the type of piece.
std::ostream& operator<<(std::ostream& os, const Piece& p);
//! \brief Print the piece color.
std::ostream& operator<<(std::ostream& os, const Color& c);

#endif
