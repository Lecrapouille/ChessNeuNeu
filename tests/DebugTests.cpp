//=====================================================================
// ChessNeuNeu: Non serious chess engine for learning neural networks.
// Copyright 2018 -- 2022 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of ChessNeuNeu.
//
// SimTaDyn is free software: you can redistribute it and/or modify it
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

#include "main.hpp"
#include "Chess/Rules.hpp"
#include <iostream>
#include <sstream>

//------------------------------------------------------------------------------
TEST(DebugTests, charTopiece)
{
    ASSERT_EQ(WhitePawn, char2Piece('P'));
    ASSERT_EQ(WhiteBishop, char2Piece('B'));
    ASSERT_EQ(WhiteKnight, char2Piece('N'));
    ASSERT_EQ(WhiteRook, char2Piece('R'));
    ASSERT_EQ(WhiteKing, char2Piece('K'));
    ASSERT_EQ(WhiteQueen, char2Piece('Q'));

    ASSERT_EQ(BlackPawn, char2Piece('p'));
    ASSERT_EQ(BlackBishop, char2Piece('b'));
    ASSERT_EQ(BlackKnight, char2Piece('n'));
    ASSERT_EQ(BlackRook, char2Piece('r'));
    ASSERT_EQ(BlackKing, char2Piece('k'));
    ASSERT_EQ(BlackQueen, char2Piece('q'));

    ASSERT_EQ(NoPiece, char2Piece('l'));
}

//------------------------------------------------------------------------------
TEST(DebugTests, pieceTochar)
{
    ASSERT_EQ('p', piece2char(WhitePawn));
    ASSERT_EQ('p', piece2char(BlackPawn));
    ASSERT_EQ('b', piece2char(WhiteBishop));
    ASSERT_EQ('b', piece2char(BlackBishop));
    ASSERT_EQ('n', piece2char(WhiteKnight));
    ASSERT_EQ('n', piece2char(BlackKnight));
    ASSERT_EQ('r', piece2char(WhiteRook));
    ASSERT_EQ('r', piece2char(BlackRook));
    ASSERT_EQ('k', piece2char(WhiteKing));
    ASSERT_EQ('k', piece2char(BlackKing));
    ASSERT_EQ('q', piece2char(WhiteQueen));
    ASSERT_EQ('q', piece2char(BlackQueen));
}

//------------------------------------------------------------------------------
TEST(DebugTests, Operator1)
{
    std::stringstream out;

    out << Color::White;
    ASSERT_EQ("White", out.str());

    out.str(std::string());
    out << Color::Black;
    ASSERT_EQ("Black", out.str());
}

//------------------------------------------------------------------------------
TEST(DebugTests, Operator2)
{
    std::stringstream out;

    out << PieceType::Pawn;
    ASSERT_EQ("P", out.str());

    out.str(std::string());
    out << PieceType::Bishop;
    ASSERT_EQ("B", out.str());

    out.str(std::string());
    out << PieceType::Knight;
    ASSERT_EQ("N", out.str());

    out.str(std::string());
    out << PieceType::Rook;
    ASSERT_EQ("R", out.str());

    out.str(std::string());
    out << PieceType::Queen;
    ASSERT_EQ("Q", out.str());

    out.str(std::string());
    out << PieceType::King;
    ASSERT_EQ("K", out.str());
}

//------------------------------------------------------------------------------
TEST(DebugTests, Operator3)
{
    std::stringstream out;

    out << WhitePawn;
    ASSERT_EQ("P", out.str());

    out.str(std::string());
    out << WhiteBishop;
    ASSERT_EQ("B", out.str());

    out.str(std::string());
    out << WhiteKnight;
    ASSERT_EQ("N", out.str());

    out.str(std::string());
    out << WhiteRook;
    ASSERT_EQ("R", out.str());

    out.str(std::string());
    out << WhiteQueen;
    ASSERT_EQ("Q", out.str());

    out.str(std::string());
    out << WhiteKing;
    ASSERT_EQ("K", out.str());

    out.str(std::string());
    out << BlackPawn;
    ASSERT_EQ("p", out.str());

    out.str(std::string());
    out << BlackBishop;
    ASSERT_EQ("b", out.str());

    out.str(std::string());
    out << BlackKnight;
    ASSERT_EQ("n", out.str());

    out.str(std::string());
    out << BlackRook;
    ASSERT_EQ("r", out.str());

    out.str(std::string());
    out << BlackQueen;
    ASSERT_EQ("q", out.str());

    out.str(std::string());
    out << BlackKing;
    ASSERT_EQ("k", out.str());
}

//------------------------------------------------------------------------------
TEST(DebugTests, Operator4)
{
    std::stringstream out;

    out << Move("e2e4");
    ASSERT_EQ("e2-e4", out.str());

    out.str(std::string());
    out << CastleMove(sqE1, sqG1, Castle::Little); // FIXME: tester cas pathologique
    ASSERT_EQ("O-O", out.str());

    out.str(std::string());
    out << CastleMove(sqE8, sqC8, Castle::Big);
    ASSERT_EQ("O-O-O", out.str());

    out.str(std::string());
    out << PawnSimpleMove(sqD4, sqE3, sqE4);  // FIXME: tester cas pathologique
    ASSERT_EQ("d4-e3ep", out.str());

    out.str(std::string());
    out << PromoteMove(sqH7, sqH8, PieceType::Rook);  // FIXME: tester cas pathologique
    ASSERT_EQ("h7-h8:R", out.str());
}

//------------------------------------------------------------------------------
TEST(DebugTests, Operator5)
{
    std::stringstream out;

    out << Status::WhiteWon;
    ASSERT_EQ("White won", out.str());

    out.str(std::string());
    out << Status::BlackWon;
    ASSERT_EQ("Black won", out.str());

    out.str(std::string());
    out << Status::Stalemate;
    ASSERT_EQ("Stalemate", out.str());

    out.str(std::string());
    out << Status::InternalError;
    ASSERT_EQ("Internal error", out.str());

    out.str(std::string());
    out << Status::Playing;
    ASSERT_EQ("Playing", out.str());
}
