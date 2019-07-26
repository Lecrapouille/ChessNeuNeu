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

#include "NeuNeu.hpp"
#include <random>
#include <iomanip>

// Random
std::random_device rd;  // Will be used to obtain a seed for the random number engine
std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()

// Random sorting a chessboard square for pieces (except for pawns)
std::uniform_int_distribution<> disSquare(0, 63);
// Random sorting a chessboard square for pwans (excluding row 1 and 8)
std::uniform_int_distribution<> disPawn(8, 55);
// Random sorting a Piece (except dummy square)
std::uniform_int_distribution<> disPiece(1, 7);
// Random sorting a value between 0.0f and 1.0f (+/- epilson)
std::uniform_real_distribution<float> dis1(0.0f, 0.999999f);

#undef RANDOM_MOVES

NeuNeu::NeuNeu(Rules const& rules, Color const side)
  : IPlayer(PlayerType::NeuNeuIA, side), m_rules(rules)
{
  #warning "TODO Bug with Pawns: inverted color vs. inverted from/to in matrix A"
  for (uint8_t i = 1; i < 7u; ++i) // Should be 0 .. 7 instead of 1 .. 6
  {
      m_pieces[i] = new Neurone;
      learn(NeuralPiece2Piece(static_cast<NeuralPiece>(i)), *m_pieces[i]);
      //debug(static_cast<NeuralPiece>(i));
    }
}

NeuNeu::~NeuNeu()
{
  for (uint8_t piece = 0; piece < 8u; ++piece)
    delete m_pieces[piece];
}

void NeuNeu::debug(NeuralPiece const piece)
{
  auto A = m_pieces[piece]->A;

  std::cout << "Matrix:" << std::endl << "    ";

  // Header: piece movement destination
  for (uint8_t i = 0; i < NbSquares; ++i)
    std::cout << c_square_names[i] << "  ";
  std::cout << std::endl;

  for (uint8_t i = 0; i < NbSquares; ++i)
    {
      // Left border: piece movement origin
      std::cout << c_square_names[i];

      // Display synaps
      for (uint8_t j = 0; j < NbSquares; ++j)
        std::cout << "   " << A[i][j];
      std::cout << std::endl;
    }

  /*std::cout << "Probabilities: " << std::endl;
  for (uint8_t i = 0; i < NbSquares; ++i)
    std::cout << (int) i << ": " << std::fixed << std::setprecision(6) << q[i] << std::endl;
  */
}

// Do not allow placing pawns on row 1 or 8
bool NeuNeu::isValidPawnPosition(uint8_t const from) const
{
  return (from >= 8) && (from <= 55);
}

void NeuNeu::abort()
{
  //FIXME
}

uint8_t NeuNeu::play(uint8_t const from, Neurone &neurone, bool const rand_move)
{
  float e[NbSquares];
  float q[NbSquares];
  auto A = neurone.A;

  // Place the same piece on the input vector.
  // 1: for the piece. 0: for empty squares.
  for (uint8_t i = 0; i < NbSquares; ++i)
    e[i] = 0.0f;
  e[from] = 1.0f;

  // Matrix product q = A * e
  for (uint8_t i = 0; i < NbSquares; ++i)
    q[i] = 0.0f;
  for (uint8_t i = 0; i < NbSquares; ++i)
    for (uint8_t j = 0; j < NbSquares; ++j)
      q[i] += (A[i][j] * e[j]); // FIXME: from/to inverse !

  // Normalize q
  float sum = 0.0f;
  for (uint8_t i = 0; i < NbSquares; ++i)
    sum += q[i];
  //assert(sum != 0.0f); // Because of Pawns
  sum = 0.000001f;
  for (uint8_t i = 0; i < NbSquares; ++i)
    q[i] /= sum;

  if (!rand_move)
    return 0;

  // Random the move destination
  // It's: ok that destination can be the origin
  float y = dis1(gen);
  uint8_t to;
  float p = 0.0f;
  for (to = 0; to < NbSquares; ++to)
    {
      p += q[to];
      if (p >= y)
        break;
    }
  if (to >= NbSquares)
    to = 63u;
  //assert(to <= 63u);
  return to;
}

void NeuNeu::learn(Piece piece, Neurone &neurone)
{
  Rules local_rules;
  chessboard board;
  bool res;
  auto A = neurone.A;

  // Hack: allow not having Kings on the chessboard
  // Which is not allowed by chess rules.
  if (piece.type() != PieceType::King)
    local_rules.m_no_kings = true;

  // Init Matrix
  for (uint8_t i = 0; i < NbSquares; ++i)
    for (uint8_t j = 0; j < NbSquares; ++j)
      A[i][j] = 1.0f;

#ifdef RANDOM_MOVES
  for (uint32_t it = 0; it < c_max_iterations; ++it)
    {
      uint8_t from = (piece.type() == PieceType::Pawn) ? disPawn(gen) : disSquare(gen);
#else
  for (uint8_t from = 0; from < NbSquares; ++from)
    for (uint8_t to = 0; to < NbSquares; ++to)
#endif
      {
        if ((piece.type() == PieceType::Pawn) && (!isValidPawnPosition(from)))
          {
            res = false;
            goto l_update_A;
          }

        // Place a single piece on a dummy chessboard
        board = Chessboard::Empty;
        board[from] = piece;
        local_rules.m_side = static_cast<Color>(piece.m_color);
        local_rules.m_board = board;
        local_rules.generateValidMoves();

        //
#ifdef RANDOM_MOVES
        to = play(from, neurone, true);
#else
        play(from, neurone, false);
#endif

        // Increment or decrement the weight of the move
        // depending if its a legal or illegal move.
        // Negative values are directly killed.
        res = local_rules.isValidMove(toStrMove(from, to));

      l_update_A:
#ifdef RANDOM_MOVES
        A[from][to] += (res ? 1.0f : -1.0f);
        if (A[from][to] < 0.0f)
          A[from][to] = 0.0f;
#else
        if (!res)
          A[from][to] = 0.0f;
#endif
    }
}

NeuralPiece NeuNeu::Piece2NeuralPiece(Piece const piece) const
{
  switch (piece.type())
    {
    case PieceType::Pawn:
      if (piece.color() == Color::Black)
        return NeuralPiece::NeuralBlackPawn;
      return NeuralPiece::NeuralWhitePawn;
    case PieceType::Rook:
      return NeuralPiece::NeuralRook;
    case PieceType::Bishop:
      return NeuralPiece::NeuralBishop;
    case PieceType::Knight:
      return NeuralKnight;
    case PieceType::Queen:
      return NeuralPiece::NeuralQueen;
    case PieceType::King:
      return NeuralPiece::NeuralKing;
    case PieceType::Empty:
    case PieceType::NotUsed:
    default:
      break;
    }

  assert(1 && "Unknown conversion Piece to NeuralPiece");
  return NeuralPiece::NeuralEmptyPiece;
}

Piece NeuNeu::NeuralPiece2Piece(NeuralPiece const piece) const
{
  switch (piece)
    {
    case NeuralPiece::NeuralBlackPawn:
      return BlackPawn;
    case NeuralPiece::NeuralWhitePawn:
      return WhitePawn;
    case NeuralPiece::NeuralRook:
      return WhiteRook;
    case NeuralPiece::NeuralBishop:
      return WhiteBishop;
    case NeuralPiece::NeuralKnight:
      return WhiteKnight;
    case NeuralPiece::NeuralQueen:
      return WhiteQueen;
    case NeuralPiece::NeuralKing:
      return WhiteKing;
    case NeuralPiece::NeuralEmptyPiece:
    default:
      break;
    }

  assert(1 && "Unknown conversion Piece to NeuralPiece");
  return NoPiece;
}

// FIXME mettre en place la liste des pieces dans Rules pour eviter de rechercher une piece
std::string NeuNeu::play()
{
  // No move possible
  if (size_t(0) == m_rules.m_legal_moves.size())
    return "error";

  // Get a type of piece to play
l_find_piece:
  PieceType const p = static_cast<PieceType>(disPiece(gen));

  if ((p == PieceType::Empty) || (p == PieceType::NotUsed))
    goto l_find_piece;

  // Search the piece of the chessboard
  uint8_t from = 255;
  for (uint8_t i = 0u; i < NbSquares; ++i)
    {
      if ((m_rules.m_board[i].type() == p) &&
          (m_rules.m_board[i].color() == m_rules.m_side))
        {
          from = i;
          break;
        }
    }
  if (from == 255)
    goto l_find_piece;

  // Play the piece
  uint8_t to = play(from, *m_pieces[p], true);

  // TODO by neural network
  if (!m_rules.isValidMove(toStrMove(from, to)))
    goto l_find_piece;

  if (from == to)
    goto l_find_piece;

  return toStrMove(from, to);
}
