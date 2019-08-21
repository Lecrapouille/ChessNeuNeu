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

//! \file See the document doc/ChessNeuNeu.pdf for understanding its code.

//------------------------------------------------------------------------------
// Random
std::random_device rd;  // Will be used to obtain a seed for the random number engine
std::mt19937 generator(rd()); // Standard mersenne_twister_engine seeded with rd()
// Random sorting a chessboard square for pieces (except for pawns)
std::uniform_int_distribution<> randomSquare(0, NbSquares - 1u);
// Random sorting a chessboard square for pwans (excluding row 1 and 8)
std::uniform_int_distribution<> randomPawn(sqA7, sqH2);
// Random sorting a value between 0.0f and 1.0f (+/- epilson)
std::uniform_real_distribution<float> randomProba(0.0f, 1.0f);

//------------------------------------------------------------------------------
//! \brief Print the type of piece.
//------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const NeuralPiece& p)
{
  switch(p)
    {
    case NeuralRook:
      os << "Rook";
      break;
    case NeuralKnight:
      os << "Knight";
      break;
    case NeuralBishop:
      os << "Bishop";
      break;
    case NeuralQueen:
      os << "Queen";
      break;
    case NeuralKing:
      os << "King";
      break;
    case NeuralWhitePawn:
      os << "White Pawn";
      break;
    case NeuralBlackPawn:
      os << "Black Pawn";
      break;
    case NeuralEmpty:
    default:
      break;
    }
  return os;
}

//------------------------------------------------------------------------------
//! \brief Define RANDOM_MOVES for a classic random training.
//! -- Define RANDOM_MOVES for a more classic neural network training.
//!    Need > 100,000 iterations.
//! -- Undefine RANDOM_MOVES for a hacked and faster version of training
//!    (testing systematic cases).  RANDOM_MOVES make millions of iterations
//!    while the hack version is 4096 iterations.
#undef RANDOM_MOVES

//------------------------------------------------------------------------------
//! \brief Define this macro for debuging synaps.
#define DISPLAY_SYNAPS

//------------------------------------------------------------------------------
NeuNeu::NeuNeu(const Rules &rules, const Color side)
  : IPlayer(PlayerType::NeuNeuIA, side), m_rules(rules)
{
  // Iterate on type of figures
  for (uint8_t i = 0u; i < 8u; ++i)
  {
      m_neurons[i] = new Synaps;
      NeuralPiece np = static_cast<NeuralPiece>(i);
      if (NeuralPiece::NeuralEmpty != np)
        {
          trainSynaps(NeuralPiece2Piece(np), *m_neurons[i]);
          showSynaps(np);
        }
    }
}

//------------------------------------------------------------------------------
NeuNeu::~NeuNeu()
{
  for (uint8_t piece = 0u; piece < 8u; ++piece)
    delete m_neurons[piece];
}

//------------------------------------------------------------------------------
void NeuNeu::showSynaps(const NeuralPiece piece)
{
  auto const& A = m_neurons[piece]->weights;

  std::cout << "Synaps of " << piece << ":" << std::endl << "  ";
  std::ios_base::fmtflags f(std::cout.flags());

  // Header: piece movement destination
  for (uint8_t i = 0; i < NbSquares; ++i)
    std::cout << std::setw(6) << c_square_names[i];
  std::cout << std::endl;

  for (uint8_t i = 0; i < NbSquares; ++i)
    {
      // Left border: piece movement origin
      std::cout << c_square_names[i];

      // Display weight of synaps
      for (uint8_t j = 0; j < NbSquares; ++j)
        std::cout << std::setw(6) << (int) A[i][j];
      std::cout << std::endl;
    }
  std::cout.flags(f);
}

//------------------------------------------------------------------------------
void NeuNeu::showProbabilities(const uint8_t from)
{
  std::cout << "Probabilities: " << std::endl;

  std::ios_base::fmtflags f(std::cout.flags());
  std::cout << std::fixed << std::setprecision(6);

  for (uint8_t to = 0; to < NbSquares; ++to)
    {
      std::cout << "  " << toStrMove(from, to) << ": "
                << q[to] << std::endl;
    }
  std::cout.flags(f);
}

//------------------------------------------------------------------------------
uint8_t NeuNeu::synapsPlay(const uint8_t from, Synaps &synaps)
{
  auto const& A = synaps.weights;

  // Place the same piece on the input vector:
  // 1.0f for the piece.
  // 0.0f for empty squares.
  for (uint8_t i = 0; i < NbSquares; ++i)
    e[i] = 0.0f;
  e[from] = 1.0f;

  // Matrix product q = A * e
  for (uint8_t i = 0; i < NbSquares; ++i)
    q[i] = 0.0f;
  for (uint8_t i = 0; i < NbSquares; ++i)
    for (uint8_t j = 0; j < NbSquares; ++j)
      q[i] += (A[i][j] * e[j]);

  // Normalize q
  {
    float sum = 0.0f;
    for (uint8_t i = 0; i < NbSquares; ++i)
      sum += q[i];

    // Avoid possible division by 0 with pawns
    // FIXME if ((NeuralBlackPawn == TODO) || (NeuralWhitePawn == TODO))
      {
        sum += 0.000001f;
      }
    assert(sum != 0.0f);

    for (uint8_t i = 0; i < NbSquares; ++i)
      q[i] /= sum;
  }

  // TODO Optimization: when doing all cases (not random) of possible movements
  // We can avoid to get destination of the movement because it's not used.
  // #ifdef !RANDOM_MOVES
  //   if (!learning) return 0;
  // #endif

  // Random the move destination
  // It's: ok that destination can be the origin
  float y = randomProba(generator);
  float p = 0.0f;
  uint8_t to;
  for (to = 0; to < NbSquares; ++to)
    {
      p += q[to];
      if (p >= y)
        break;
    }

  return (to < NbSquares) ? to : uint8_t(Square::OOB);
}

//------------------------------------------------------------------------------
void NeuNeu::trainSynaps(Piece piece, Synaps &synaps)
{
  Rules local_rules;
  chessboard board;
  bool res;
  std::string move;
  auto& A = synaps.weights;

  // Hack: allow not having Kings on the chessboard
  // Which is not allowed by chess rules.
  if (piece.type != PieceType::King)
    local_rules.m_no_kings = true;

  // Init Matrix
  for (uint8_t i = 0; i < NbSquares; ++i)
    for (uint8_t j = 0; j < NbSquares; ++j)
      A[i][j] = 1.0f;

#ifdef RANDOM_MOVES
  uint8_t to;
  const uint32_t c_max_iterations = 1000000u;
  for (uint32_t it = 0; it < c_max_iterations; ++it)
    {
      uint8_t from = (piece.type == PieceType::Pawn) ? randomPawn(generator) : randomSquare(generator);
#else
  for (uint8_t from = 0; from < NbSquares; ++from)
    for (uint8_t to = 0; to < NbSquares; ++to)
      {
#endif
        if ((piece.type == PieceType::Pawn) && (!isValidPawnPosition(from)))
          {
            res = false;
            goto l_update_A;
          }

        // Place a single piece on a dummy chessboard
        board = Chessboard::Empty;
        board[from] = piece;
        local_rules.m_side = static_cast<Color>(piece.color);
        local_rules.m_board = board;
        local_rules.generateValidMoves();

        // Generate a move made by synaps.
#ifdef RANDOM_MOVES
        to = synapsPlay(from, synaps);
        assert(Square::OOB != to);
#  ifdef DISPLAY_SYNAPS
        //std::cout << toStrMove(from, to) << std::endl;
#  endif
#else
        synapsPlay(from, synaps);
        // 'to' is ignored in this optimized case.
#endif

        // Increment or decrement the weight of the move depending if its a
        // legal or illegal move. Negative values are directly killed.
        move = toStrMove(from, to);
        if ((piece.type == PieceType::Pawn) && !isValidPawnPosition(to))
          {
            // Promote case
            move += "Q";
          }
        res = local_rules.isValidMove(move);

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

//------------------------------------------------------------------------------
// FIXME mettre en place la liste des pieces dans Rules pour eviter de
// rechercher une piece
std::string NeuNeu::play()
{
  // No move possible
  int len = m_rules.m_legal_moves.size();
  if (len == 0)
    return IPlayer::error;

  // Get the list of squares where figures are present
  std::vector<uint8_t> figures;
  figures.reserve(16);
  for (uint8_t i = 0u; i < NbSquares; ++i)
    {
      if (m_rules.m_board[i].color == m_rules.m_side)
        figures.push_back(i);
    }
  assert(0u != figures.size());

  // Randomize the origin of the movement
l_find_piece:
  std::uniform_int_distribution<> randomFigure(0u, figures.size() - 1u);
  int rr = randomFigure(generator);
  std::cout << "RANDOM " << rr << std::endl;
  uint8_t from = figures[rr];
  PieceType p = static_cast<PieceType>(m_rules.m_board[from].type);
  assert(PieceType::Empty != p);

  // Complete the destination of the move
  uint8_t to = synapsPlay(from, *m_neurons[p]);

  // Check if a valid move. Else retry a new random move
  if ((Square::OOB == to) || (from == to))
    goto l_find_piece;

  if (!m_rules.isValidMove(toStrMove(from, to)))
    goto l_find_piece;

  // Show probabilites of movement
#ifdef DISPLAY_SYNAPS
  showProbabilities(from);
#endif

  return toStrMove(from, to);
}
