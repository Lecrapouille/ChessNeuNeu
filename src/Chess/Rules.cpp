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

#include "Chess/Rules.hpp"
#include <valarray>
#include <sstream>

//! \brief
static const std::array<uint8_t, 64> c_mailbox64 =
  {{
      21u, 22u, 23u, 24u, 25u, 26u, 27u, 28u,
      31u, 32u, 33u, 34u, 35u, 36u, 37u, 38u,
      41u, 42u, 43u, 44u, 45u, 46u, 47u, 48u,
      51u, 52u, 53u, 54u, 55u, 56u, 57u, 58u,
      61u, 62u, 63u, 64u, 65u, 66u, 67u, 68u,
      71u, 72u, 73u, 74u, 75u, 76u, 77u, 78u,
      81u, 82u, 83u, 84u, 85u, 86u, 87u, 88u,
      91u, 92u, 93u, 94u, 95u, 96u, 97u, 98u
    }};

//! \brief
static const std::array<uint8_t, 120> c_mailbox120 =
  {{
      OOB, OOB, OOB, OOB, OOB, OOB, OOB, OOB, OOB, OOB,
      OOB, OOB, OOB, OOB, OOB, OOB, OOB, OOB, OOB, OOB,
      OOB,  0u,  1u,  2u,  3u,  4u,  5u,  6u,  7u, OOB,
      OOB,  8u,  9u, 10u, 11u, 12u, 13u, 14u, 15u, OOB,
      OOB, 16u, 17u, 18u, 19u, 20u, 21u, 22u, 23u, OOB,
      OOB, 24u, 25u, 26u, 27u, 28u, 29u, 30u, 31u, OOB,
      OOB, 32u, 33u, 34u, 35u, 36u, 37u, 38u, 39u, OOB,
      OOB, 40u, 41u, 42u, 43u, 44u, 45u, 46u, 47u, OOB,
      OOB, 48u, 49u, 50u, 51u, 52u, 53u, 54u, 55u, OOB,
      OOB, 56u, 57u, 58u, 59u, 60u, 61u, 62u, 63u, OOB,
      OOB, OOB, OOB, OOB, OOB, OOB, OOB, OOB, OOB, OOB,
      OOB, OOB, OOB, OOB, OOB, OOB, OOB, OOB, OOB, OOB
    }};

enum Rosas { N = -10, W = -1, E = 1, S = 10 };

//! \brief Define for each pieces their displacement in the c_mailbox64.
//! Let's suppose a King is placed on the chessboard square e4. This
//! corresponds to the square 65 of the c_mailbox64. The King can moves to
//! squares d3, e3, f3, f4, f5, e5, d5 and d4 which corresponds to squares
//! 64, 75, 76, 66, 56, 55, 54 and 64 in c_mailbox64. Relative displacement
//! is their difference with the 65. For example -11 = 54 - 65. For other
//! pieces which can slide we store the displacement of a distance of 1.
static const std::valarray<int> c_relative_movements[8] =
  {
    [PieceType::Empty]  = { },
    [PieceType::Rook]   = { N, E, S, W },
    [PieceType::Knight] = { N+N+E, N+E+E, S+E+E, S+S+E, S+S+W, S+W+W, N+W+W, N+N+W },
    [PieceType::Bishop] = { N+E, N+W, S+E, S+W },
    [PieceType::Queen]  = { N, S, E, W, N+W, N+E, S+E, S+W },
    [PieceType::King]   = { N, S, E, W, N+W, N+E, S+E, S+W },
    [PieceType::WPawn]  = { N, N+N, N+E, N+W },
    [PieceType::BPawn]  = { S, S+S, S+E, S+W },
  };

Rules::Rules()
  : m_status(Status::Playing),
    m_side(Color::White),
    m_board(Chessboard::Init),
    m_no_kings(WithKings),
    m_ep(Square::OOB)
{
  m_castle[Color::White] = Castle::Both;
  m_castle[Color::Black] = Castle::Both;
  generateValidMoves();
  saveInit();
}

Rules::Rules(const chessboard &board, const Color side,
             bool noking, Castle wcastle, Castle bcastle,
             uint8_t ep)
  : m_status(Status::Playing),
    m_side(side),
    m_board(board),
    m_no_kings(noking),
    m_ep(ep)
{
  if (noking == WithNoKings)
    {
      m_castle[Color::White] = Castle::NoCastle;
      m_castle[Color::Black] = Castle::NoCastle;
    }
  else
    {
      m_castle[Color::White] = wcastle;
      m_castle[Color::Black] = bcastle;
    }
  generateValidMoves();
  saveInit();
}

Rules::Rules(std::string const& fen)
{
  if (false == import(fen))
    {
      throw std::string("Incorrect FEN string");
    }
  generateValidMoves();
  saveInit();
}

bool Rules::load(const std::string& moves, const bool init_board)
{
  if (init_board)
    {
      restoreInit();
    }
  m_moved.clear();
  generateValidMoves();

  std::istringstream iss(moves);
  while (iss)
    {
      std::string move;
      iss >> move;

      if (move.empty())
        return true;

      if (!applyMove(move))
        return false;
    }

  saveInit();
  return true;
}

bool Rules::load(std::string const& fen)
{
  if (false == import(fen))
    return false;

  generateValidMoves();
  saveInit();
  return true;
}

void Rules::restoreInit()
{
  m_board = m_initial.board;
  m_side = m_initial.side;
  m_ep = m_initial.ep;
  m_castle[0] = m_initial.castle[0];
  m_castle[1] = m_initial.castle[1];
}

void Rules::saveInit()
{
  m_initial.board = m_board;
  m_initial.side = m_side;
  m_initial.ep = m_ep;
  m_initial.castle[0] = m_castle[0];
  m_initial.castle[1] = m_castle[1];
}

const std::vector<Move>& Rules::generatePseudoValidMoves()
{
  Piece p;
  PieceType pt;

  m_pseudo_moves.clear();

  for (uint8_t ij = 0u; ij < NbSquares; ++ij)
    {
      p = m_board[ij];

      // Ignore: empty position and pieces of the opposite side
      if ((p.type == PieceType::Empty) || (p.color != m_side))
        continue ;

      pt = static_cast<PieceType>(p.type);

      // Pawn moves
      if (pt == PieceType::Pawn)
        {
          if (m_side == Color::Black)
            {
              // This is an hack: we use the slot of
              // PieceType::NotUsed !
              pt = PieceType::BPawn;
            }
          generatePseudoLegalPawnMove(ij, pt);
        }
      else // Major piece movements
        {
          generatePseudoLegalPieceMove(ij, pt);
        }
    }

  // Castling
  if (m_castle[m_side] != 0)
    {
      generatePseudoLegalCastleMove();
    }

  //dispPseudoMoves();
  return m_pseudo_moves;
}

// FIXME passer cdirectement _relative_movements[pt]
void Rules::generatePseudoLegalPawnMove(const uint8_t from, const PieceType pt)
{
  Piece piece;
  for (uint8_t mvt = 0u; mvt < c_relative_movements[pt].size(); ++mvt)
    {
      // Invalid move: North+North from squaree != [A-H]2 or [A-H]7
      uint8_t r = ROW(from);
      if ((mvt == 1u) && (r != 1) && (r != 6))
        continue;

      // One step relative movement
      uint8_t to = c_mailbox120[c_mailbox64[from] + c_relative_movements[pt][mvt]];

      // Invalid move: outside the chessboard
      if (to == Square::OOB)
        continue;

      piece = m_board[to];

      // Invalid move: move to a piece with the same color
      if (piece.color == m_side)
        continue;

      // Invalid move: North and North+North
      if ((mvt <= 1u) && (piece.type != PieceType::Empty))
        continue;

      // Invalid diagonal move (take) if no piece and not en-passant
      if ((mvt > 1u) && (piece.type == PieceType::Empty) && (to != m_ep))
        continue;

      // Promotion
      r = ROW(to);
      if ((r == 0) || (r == 7))
        {
          for (int promote = PieceType::Rook; promote <= PieceType::Queen; ++promote)
            {
              m_pseudo_moves.push_back(PromoteMove(from, to, static_cast<PieceType>(promote)));
            }
        }
      else // Normal move
        {
          if (mvt == 1)
            {
              m_pseudo_moves.push_back(PawnDoubleMove(from, to));
            }
          else
            {
              m_pseudo_moves.push_back(PawnSimpleMove(from, to, to == m_ep));
            }
        }
    }
}

void Rules::generatePseudoLegalPieceMove(const uint8_t from, const PieceType pt)
{
  Piece piece;
  const bool can_slide = m_board[from].slide;

  for (uint8_t mvt = 0u; mvt < c_relative_movements[pt].size(); ++mvt)
    {
      for (uint8_t to = from;;)
        {
          // One step relative movement
          to = c_mailbox120[c_mailbox64[to] + c_relative_movements[pt][mvt]];

          // Invalid move: outside the chessboard
          if (to == Square::OOB)
            break;

          piece = m_board[to];

          // Invalid move: move to a piece with the same color
          if (piece != NoPiece)
            {
              // Piece takes other piece
              if (piece.color != m_side)
                {
                  m_pseudo_moves.push_back(PieceMove(from, to));
                }
              break;
            }

          // Arrive to an empty square or to an opposite piece
          m_pseudo_moves.push_back(PieceMove(from, to));

          // If cannot do more than one relative movement
          if (can_slide == false)
            break;
        }
    }
}

void Rules::generatePseudoLegalCastleMove()
{
  Color xside = opposite(m_side);
  const uint8_t offset = (m_side == Color::White) ? 0 : (sqE1 - sqE8);

  // No Kings = no castle
  if (m_no_kings)
    return ;

  // Castle not possible if King is in check
  if (isKingInCheck(m_board, m_side))
    return ;

  // King castle
  if ((m_castle[m_side] & Castle::Little) &&
      (m_board[sqF1 - offset].type == PieceType::Empty) &&
      (m_board[sqG1 - offset].type == PieceType::Empty) &&
      (!attack(m_board, sqF1 - offset, xside)))
    {
      m_pseudo_moves.push_back(CastleMove(sqE1 - offset,
                                          sqG1 - offset,
                                          Castle::Little));
    }

  // Queeen castle
  if ((m_castle[m_side] & Castle::Big) &&
      (m_board[sqD1 - offset].type == PieceType::Empty) &&
      (m_board[sqB1 - offset].type == PieceType::Empty) &&
      (m_board[sqC1 - offset].type == PieceType::Empty) &&
      (!attack(m_board, sqD1 - offset, xside)))
    {
      m_pseudo_moves.push_back(CastleMove(sqE1 - offset,
                                          sqC1 - offset,
                                          Castle::Big));
    }
}

// generatePseudoValidMoves() shall be called before
const std::vector<Move>& Rules::generateValidMoves()
{
  m_legal_moves.clear();
  generatePseudoValidMoves();

  for (const auto it: m_pseudo_moves)
    {
      if (tryMove(it))
        {
          m_legal_moves.push_back(it);
        }
    }

  updateGameStatus();
  //dispLegalMoves();
  return m_legal_moves;
}

// generateValidMoves() shall be called before
bool Rules::isValidMove(std::string const& move) const
{
  Move m(move);

  for (const auto it: m_legal_moves)
    {
      if (it == m)
        {
          return true;
        }
    }
  return false;
}

// generateValidMoves() shall be called before
void Rules::dispLegalMoves() const
{
  std::cout << "Legal moves: " << std::endl;
  for (const auto it: m_legal_moves)
    {
      std::cout << "  " << it << std::endl;
    }
  std::cout << std::endl;
}

// generateValidMoves() shall be called before
void Rules::dispPseudoMoves() const
{
  std::cout << "Pseudo moves: " << std::endl;
  for (const auto it: m_pseudo_moves)
    {
      std::cout << "  " << it << std::endl;
    }
  std::cout << std::endl;
}

bool Rules::tryMove(const Move move) const
{
  //! \brief Temporary board for computations.
  chessboard board = this->m_board;

  // Simulate the next move.
  updateBoard(move, board);

  return !isKingInCheck(board, m_side);
}

// FIXME: Optim memoriser la position du roi au lieu de la retrouver
bool Rules::isKingInCheck(chessboard const& board, const Color side) const
{
  // Special case for Neural network using empty chessboard with no Kings
  if (m_no_kings)
    return false;

  for (uint8_t i = 0u; i < NbSquares; ++i)
    {
      if ((board[i].type == PieceType::King) &&
          (board[i].color == side))
        {
          return attack(board, i, opposite(side));
        }
    }
  assert(1 && "isKingInCheck() did not find the King");
  return false;
}

bool Rules::attack(const chessboard& position, const uint8_t sq, const Color side) const
{
  int n;

  for (uint8_t i = 0u; i < NbSquares; ++i)
    {
      const Piece pinfo = position[i];
      if ((pinfo.type == PieceType::Empty) || (pinfo.color != side))
        continue;

      const PieceType piece = static_cast<PieceType>(pinfo.type);
      if (piece == PieceType::Pawn)
        {
          if (side == Color::White)
            {
              if (COL(i) != 0 && i - 9 == sq)
                return true;
              if (COL(i) != 7 && i - 7 == sq)
                return true;
            }
          else
            {
              if (COL(i) != 0 && i + 7 == sq)
                return true;
              if (COL(i) != 7 && i + 9 == sq)
                return true;
            }
        }
      else
        {
          for (uint8_t j = 0u; j < c_relative_movements[piece].size(); ++j)
            {
              for (n = i;;)
                {
                  n = c_mailbox120[c_mailbox64[n] + c_relative_movements[piece][j]];

                  if (n == Square::OOB)
                    break;

                  if (n == sq)
                    return true;

                  if (position[n].type != PieceType::Empty)
                    break;

                  if (!pinfo.slide)
                    break;
                }
            }
        }
    }
  return false;
}

void Rules::applyMove(Move const& move)
{
  //FIXME
  // if (m_status != Status::Playing) return ;

  uint8_t from = move.from;
  uint8_t to = move.to;

  //FIXME
  //assert(legal_moves_generated);
  //assert(isAllowed(move))

  // Add the move to the game notes
  if (!m_moved.empty())
    m_moved += ' ';
  m_moved += c_square_names[from];
  m_moved += c_square_names[to];
  if (move.promote != PieceType::Empty)
    {
      m_moved += piece2char(static_cast<PieceType>(move.promote));
    }

  // Refresh the chessboard
  updateBoard(move, m_board);

  // Update castle status:
  // if King moved then no longer castle available
  if (m_board[to].type == PieceType::King)
    {
      m_castle[m_board[to].color] = Castle::NoCastle;
    }

  // Update castle status:
  // if Rook moved: castle can no longer be done
  // on the rook side
  else if (m_board[to].type == PieceType::Rook)
    {
      if ((to == sqA1) || (to == sqA8))
        {
          m_castle[m_side] &= ~Castle::Big;
        }
      else if ((to == sqH1) || (to == sqH8))
        {
          m_castle[m_side] &= ~Castle::Little;
        }
    }

  // Update en passant status
  else if (move.double_move)
    {
      if (m_side == Color::White)
        {
          m_ep = to + 8;
        }
      else
        {
          m_ep = to - 8;
        }
    }
  else
    {
      m_ep = Square::OOB;
    }

  // Switch color to play and generate legal moves
  m_side = opposite(m_side);
  generateValidMoves();
}

bool Rules::applyMove(std::string const& move)
{
  Move m(move);

  for (auto it: m_legal_moves)
    {
      if (it == m)
        {
          // Note: use it not m because it contains
          // more informations because computed by
          // Rules class.
          applyMove(it);
          return true;
        }
    }

  //assert(1 && "No valid moves found");
  std::cerr << "Cannot apply illegal move '"
            << move << "'" << std::endl;
  return false;
}

std::string Rules::revertLastMove()
{
  if (m_moved.empty())
    return "";

  auto pos = m_moved.rfind(" ");
  pos = (std::string::npos == pos) ? 0 : pos + 1;

  std::string last_move(m_moved.substr(pos));
  std::cout << opposite(m_side) << " reverted the move '"
            << last_move << "'" << std::endl;

  std::string move(m_moved.erase(pos));
  bool res = load(move, true);
  assert(res && "Failed reverting last move");
  return last_move;
}

void Rules::updateBoard(Move const& move, chessboard& board) const
{
  uint8_t from = move.from;
  uint8_t to   = move.to;

  // Basic movement
  board[to] = board[from];
  board[to].moved = true;

  // Promotion
  if (move.promote != PieceType::Empty)
    {
      board[to].type = move.promote;
      board[to].color = board[from].color;
      board[to].slide = (move.promote != PieceType::Knight);
    }

  board[from] = NoPiece;

  // Castle: move the rook
  if (move.castle != Castle::NoCastle)
    {
      const uint8_t offset = (m_side == Color::White) ? 0 : (sqE1 - sqE8);
      if (move.castle & Castle::Little)
        {
          board[sqF1 - offset] = board[sqH1 - offset];
          board[sqF1 - offset].moved = true;
          board[sqH1 - offset] = NoPiece;
        }
      else if (move.castle & Castle::Big)
        {
          board[sqD1 - offset] = board[sqA1 - offset];
          board[sqD1 - offset].moved = true;
          board[sqA1 - offset] = NoPiece;
        }
    }

  // En passant move
  else if (move.ep)
    {
      assert(m_ep != Square::OOB);
      if (m_side == Color::White)
        {
          board[m_ep + 8] = NoPiece;
        }
      else
        {
          board[m_ep - 8] = NoPiece;
        }
    }
}

// generateValidMoves() shall be called before
void Rules::updateGameStatus()
{
  if (m_legal_moves.size() != 0)
    {
      // Legal moves are possible means the game
      // is in progress.
      m_status = Status::Playing;
    }
  else if (m_no_kings)
    {
      // This is not a valid chess rule but for
      // neural network of unit tests we sometimes
      // need a chessboard without king. This hack
      // allows to return a status different from
      // stalemate.
      m_status = Status::NoMoveAvailable;
    }
  else if (isKingInCheck(m_board, m_side))
    {
      // No legal moves possible with the King in
      // check means checkmate and therefore the end
      // of the game.
      m_status = (Color::White == m_side) ?
        Status::BlackWon : Status::WhiteWon;
    }
  else
    {
      // No legal moves and the King not in check
      // that means stalemate and therefore the end
      // of the game.
      m_status = Status::Stalemate;
    }
}
