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

enum NeuralPiece { NeuralBlackPawn, NeuralRook, NeuralKnight, NeuralBishop, NeuralQueen, NeuralKing, NeuralWhitePawn };

// 64 is the number of squares on the chessboard
struct Neurone
{
  float A[64][64];
};

class NeuNeu: public IPlayer
{
public:

  NeuNeu(const Rules &rules, const Color side);
  ~NeuNeu();
  virtual std::string play() override;
  virtual void abort() override;
  void debug(const NeuralPiece piece);

private:

  uint8_t play(const uint8_t from, Neurone &neurone, bool rand_move);
  void learn(Piece piece, Neurone &neurone);
  NeuralPiece Piece2NeuralPiece(const Piece piece) const;
  Piece NeuralPiece2Piece(const NeuralPiece piece) const;
  bool isValidPawnPosition(const uint8_t from) const;

  const Rules &m_rules;
  Neurone *m_pieces[8u];
};

#endif
