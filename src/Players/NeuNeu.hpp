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
