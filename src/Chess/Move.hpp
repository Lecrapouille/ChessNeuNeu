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

//! \brief Print the type of piece.
enum Castle { NoCastle = 0u, Little = 1u, Big = 2u, Both = 3u };

// ***********************************************************************************************
//! \brief
// ***********************************************************************************************
struct Move
{
    Move()
    {}

    Move(std::string const& m)
    {
        from = toSquare(&m[0]);
        to = toSquare(&m[2]);
        promote = char2Piece(m[4]).type;
        castle = Castle::NoCastle;
        ep = false;
        check = false;
        double_move = false;
    }

    inline bool operator==(Move const &other) const
    {
        return (from == other.from) && (to == other.to)
                && (promote == other.promote);
    }

    uint8_t from;
    uint8_t to;
    struct
    {
        unsigned int castle        : 2; // enum Casttle
        unsigned int promote       : 3; // PieceType
        unsigned int ep            : 1; // En passant move
        unsigned int check         : 1; // King check
        unsigned int double_move   : 1; // pawn double move
    };
    // FIXME: ajouter car on a droit a 8 bits
    // ep: case 8bits
    // color: qui a joue
    // valid: pseudo moves --> legal moves
    // Piece qui a joue (pour prettyprint)
    // prise: bool

    //! \brief Used by the play() method when no move are available
    //! (like stalemate).
    static constexpr const char* none = "none";
};

struct CastleMove : public Move
{
    CastleMove(const uint8_t f, const uint8_t t, const Castle c)
    {
        assert((c == Castle::Little) || (c == Castle::Big));
        // && (from == sqE1) && (to == sq));

        from = f;
        to = t;
        castle = c;
        promote = PieceType::Empty;
        ep = false;
        check = false; // FIXME
        double_move = 0;
    }
};

struct PieceMove : public Move
{
    PieceMove(const uint8_t f, const uint8_t t)
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
    PawnSimpleMove(const uint8_t f, const uint8_t t, const bool e = false)
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
    PawnDoubleMove(const uint8_t f, const uint8_t t)
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
    PromoteMove(const uint8_t f, const uint8_t t, const PieceType p)
    {
        from = f;
        to = t;
        assert(from != to);

        castle = Castle::NoCastle;
        promote = p;
        ep = false;
        check = false;  // FIXME
        double_move = false;
    }
};

//! \brief Pretty print a move note.
std::ostream& operator<<(std::ostream& os, const Move& m);

#endif
