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

#ifndef MOVE_HPP
#define MOVE_HPP

#  include "Chess/Board.hpp"

//! \brief Allowed castles.
enum Castle { NoCastle = 0u, Little = 1u, Big = 2u, Both = 3u };

//! \brief Allow doing bool operations on Castle enum
static Castle& operator|=(Castle& a, const Castle& b)
{
  a = Castle(int(a) | int(b));
  return a;
}

//! \brief Allow doing bool operations on Castle enum
static Castle& operator&=(Castle& a, const Castle& b)
{
  a = Castle(int(a) & int(b));
  return a;
}

//! \brief Allow doing bool operations on Castle enum
static Castle operator~(const Castle &a)
{
  return Castle(~int(a));
}

// ***********************************************************************************************
//! \brief
// ***********************************************************************************************
struct Move
{

  Move() {}
  Move(std::string const& m)
  {
    from = toSquare(&m[0]);
    to = toSquare(&m[2]);
    promote = char2Piece(m[4]).m_type;
    castle = Castle::NoCastle;
    ep = false;
    check = false;
    double_move = false;
  }

  inline bool operator==(Move const& other) const
  {
    return (from == other.from) && (to == other.to)
      && (promote == other.promote);
  }

  uint8_t from;
  uint8_t to;
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wpedantic"
  struct
  {
    unsigned int castle        : 2; // enum Castle
    unsigned int promote       : 3; // PieceType
    unsigned int ep            : 1; // En passant move
    unsigned int check         : 1; // King check
    unsigned int double_move   : 1; // pawn double move
  };
# pragma GCC diagnostic pop
  // FIXME: ajouter car on a droit a 8 bits
  // ep: case 8bits
  // color: qui a joue
  // valid: pseudo moves --> legal moves
  // Piece qui a joue (pour prettyprint)
  // prise: bool
};

struct CastleMove : public Move
{
  CastleMove(uint8_t const f, uint8_t const t, Castle const c)
  {
    assert((c == Castle::Little) || (c == Castle::Big));
    // && (from == sqE1) && (to == sq));

    from = f;
    to = t;
    castle = static_cast<unsigned int>(c & 0x3);
    promote = static_cast<unsigned int>(PieceType::Empty & 0x7);
    ep = false;
    check = false; // FIXME
    double_move = 0;
  }
};

struct PieceMove : public Move
{
  PieceMove(uint8_t const f, uint8_t const t)
  {
    from = f;
    to =  t;
    assert(from != to);

    castle = Castle::NoCastle;
    promote = PieceType::Empty;
    ep = false;
    check = false; // FIXME
    double_move = 0;
  }
};

struct PawnSimpleMove : public Move
{
  PawnSimpleMove(uint8_t const f, uint8_t const t, bool const e = false)
  {
    from = f;
    to = t;
    assert(from != to);

    castle = Castle::NoCastle;
    promote = PieceType::Empty;
    ep = e;
    check = false; // FIXME
    double_move = false;
  }
};

struct PawnDoubleMove : public Move
{
  PawnDoubleMove(uint8_t const f, uint8_t const t)
  {
    from = f;
    to = t;
    assert(from != to);

    castle = Castle::NoCastle;
    promote = PieceType::Empty;
    ep = false;
    check = false;  // FIXME
    double_move = true;
  }
};

struct PromoteMove : public Move
{
  PromoteMove(uint8_t const f, uint8_t const t, PieceType const p)
  {
    from = f;
    to = t;
    assert(from != to);

    castle = Castle::NoCastle;
    promote = static_cast<unsigned int>(p & 0x7);
    ep = false;
    check = false;  // FIXME
    double_move = false;
  }
};

//! \brief Pretty print a move note.
std::ostream& operator<<(std::ostream& os, Move const& m);

#endif
