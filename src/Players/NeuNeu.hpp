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

#ifndef NEUNEU_HPP
#  define NEUNEU_HPP

#  include "Player.hpp"

//: \ brief Match enum PieceType
enum NeuralPiece { NeuralEmptyPiece, NeuralRook, NeuralKnight, NeuralBishop,
                   NeuralQueen, NeuralKing, NeuralWhitePawn, NeuralBlackPawn };

// 64 is the number of squares on the chessboard
struct Neurone
{
  float A[NbSquares][NbSquares];
};

// ***********************************************************************************************
//! \brief Implement a chess player. Here we are protopying a hand made neural network
//! learning by itself how to move pieces (for the moment learnt from an empty square).
//! TODO I have to write a document about it and clean the code (messy)
// ***********************************************************************************************
class NeuNeu: public IPlayer
{
public:

  NeuNeu(Rules const& rules, Color const side);
  ~NeuNeu();
  virtual std::string play() override;
  virtual void abort() override;
  void debug(NeuralPiece const piece);

private:

  uint8_t play(uint8_t const from, Neurone &neurone, bool const rand_move);
  void learn(Piece piece, Neurone &neurone);
  NeuralPiece Piece2NeuralPiece(Piece const piece) const;
  Piece NeuralPiece2Piece(NeuralPiece const piece) const;
  bool isValidPawnPosition(uint8_t const from) const;

  const Rules &m_rules;
  Neurone *m_pieces[8u];
};

#endif
