#include "Chess/Rules.hpp"
#include "Chess/FEN.hpp"
#include <valarray>
#include <cassert>

//! \brief
static const std::array<int, 64> c_mailbox64 =
  {{
      21, 22, 23, 24, 25, 26, 27, 28,
      31, 32, 33, 34, 35, 36, 37, 38,
      41, 42, 43, 44, 45, 46, 47, 48,
      51, 52, 53, 54, 55, 56, 57, 58,
      61, 62, 63, 64, 65, 66, 67, 68,
      71, 72, 73, 74, 75, 76, 77, 78,
      81, 82, 83, 84, 85, 86, 87, 88,
      91, 92, 93, 94, 95, 96, 97, 98
    }};

//! \brief
static const std::array<int, 120> c_mailbox120 =
  {{
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1,  0,  1,  2,  3,  4,  5,  6,  7, -1,
      -1,  8,  9, 10, 11, 12, 13, 14, 15, -1,
      -1, 16, 17, 18, 19, 20, 21, 22, 23, -1,
      -1, 24, 25, 26, 27, 28, 29, 30, 31, -1,
      -1, 32, 33, 34, 35, 36, 37, 38, 39, -1,
      -1, 40, 41, 42, 43, 44, 45, 46, 47, -1,
      -1, 48, 49, 50, 51, 52, 53, 54, 55, -1,
      -1, 56, 57, 58, 59, 60, 61, 62, 63, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
    }};

enum Rosas
  {
    North = -10,
    West  = -1,
    East  =  1,
    South =  10
  };

//! \brief Define for each pieces their displacement in the c_mailbox64.
//! Let's suppose a King is placed on the chessboard square e4. This
//! corresponds to the square 65 of the c_mailbox64. The King can moves to
//! squares d3, e3, f3, f4, f5, e5, d5 and d4 which corresponds to squares
//! 64, 75, 76, 66, 56, 55, 54 and 64 in c_mailbox64. Relative displacement
//! is their difference with the 65. For example -11 = 54 - 65. For other
//! pieces which can slide we store the displacement of a distance of 1.
static const std::valarray<int> c_relative_movements[8] =
  {
    [PieceType::Empty]   = { },
    [PieceType::Rook]    = { North,
                             East,
                             South,
                             West },
    [PieceType::Knight]  = { North + North + East,
                             North + East  + East,
                             South + East  + East,
                             South + South + East,
                             South + South + West,
                             South + West  + West,
                             North + West  + West,
                             North + North + West },
    [PieceType::Bishop]  = { North + East,
                             North + West,
                             South + East,
                             South + West },
    [PieceType::Queen]   = { North,
                             South,
                             East,
                             West,
                             North + West,
                             North + East,
                             South + East,
                             South + West },
    [PieceType::King]    = { North,
                             East,
                             South,
                             West,
                             North + West,
                             North + East,
                             South + East,
                             South + West },
    [PieceType::Pawn]    = { North,
                             North + North,
                             North + East,
                             North + West },
    [PieceType::NotUsed] = { }
  };

//! \brief Define pieces which can do more than one c_relative_movements
//! in a single step.
static const std::array<bool, 8> c_can_slide =
  {{
      [PieceType::Empty]   = false,
      [PieceType::Rook]    = true,
      [PieceType::Knight]  = false,
      [PieceType::Bishop]  = true,
      [PieceType::Queen]   = true,
      [PieceType::King]    = false,
      [PieceType::Pawn]    = false,
      [PieceType::NotUsed] = false,
    }};

Rules::Rules(std::string const& fen, bool noking)
  : m_status(Status::Playing),
    hasNoKing(noking)
{
  bool res = load(fen, m_board, m_side, m_ep);
  if (false == res)
    {
      throw std::string("Incorrect FEN string");
    }
  m_tmp_board = m_board; // FIXME: useless ?
  generateValidMoves();
}

void Rules::loadPosition(const chessboard& board)
{
  m_board = board;
}

void Rules::loadPosition(const std::string& moves)
{
  m_board = Chessboard::Init;

  for (uint32_t i = 0u; i < moves.length(); i += 5u)
    {
      makeMove(moves.substr(i, 4u));
    }
}

// 5 is the string size for example "e2e4 "
// FIXME non 5 ou 7 a cause de la prise en passant et de la promotion
void Rules::moveBack()
{
  if (m_moved.length() >= 5)
    {
      sidePlayed();
      std::cout << m_side << " reverted the move "
                << m_moved.substr(m_moved.length() - 5, 5)
                << std::endl;
      m_moved.erase(m_moved.length() - 5, 5);
      loadPosition(m_moved);

      m_status = Status::Playing;

      std::cout << m_board << std::endl;
      std::cout << m_side << " are playing";
      Status status = generateValidMoves();
      //dispLegalMoves();
      if (status != Status::Playing)
        {
          std::cout << " and position is " << status;
        }
      std::cout << std::endl;
    }
}

// next_move shall be valid
// FIXME: coup doit etre classe Move est non pas string mais c'est a cause de stockfish
void Rules::makeMove(const std::string& next_move)
{
  //std::cout << "move: " << movement << std::endl;
  int m1 = ('8' - next_move[1]) * 8 + next_move[0] - 'a';
  int m2 = ('8' - next_move[3]) * 8 + next_move[2] - 'a';

  assert(m1 != m2);

  // Update internal position
  // FIXME: code pas fini (manque promotion, en passant)
  // et en double avec makeMovetryMove (mais sur l'echiquier final)
  m_board[m2] = m_board[m1];
  m_board[m2].moved = true;
  m_board[m1] = NoPiece;

  // Move rook if castling
  if (m1 == sqE1)
    {
      // White King castle
      if (m2 == sqG1)
        {
          m_board[sqF1] = m_board[sqH1];
          m_board[sqF1].moved = true;
          m_board[sqH1] = NoPiece;
        }
      // White Queen castle
      else if (m2 == sqC1)
        {
          m_board[sqD1] = m_board[sqA1];
          m_board[sqD1].moved = true;
          m_board[sqA1] = NoPiece;
        }
    }
  else if (m1 == sqE8)
    {
      // Black King castle
      if (m2 == sqG8)
        {
          m_board[sqF8] = m_board[sqH8];
          m_board[sqF8].moved = true;
          m_board[sqH8] = NoPiece;
        }
      // Black Queen castle
      else if (m2 == sqC8)
        {
          m_board[sqD8] = m_board[sqA8];
          m_board[sqD8].moved = true;
          m_board[sqA8] = NoPiece;
        }
    }

  // Add the note move to moved notes
  m_moved += next_move;
  m_moved += ' ';

  // Switch player
  //std::cout << m_side << " played " << next_move << std::endl;
  sidePlayed();
  //std::cout << m_board << std::endl;
  //std::cout << m_side << " are playing" << std::endl;
  generateValidMoves();
}

bool Rules::tryMove(const Move move, const Color side)
{
  uint8_t king;

  // Simulate the next move.
  // FIXME: code pas fini (manque promotion, en passant)
  // et en double avec makeMove (mais sur un echiquier temporaire)
  m_tmp_board = m_board;
  m_tmp_board[move.to] = m_tmp_board[move.from];
  m_tmp_board[move.to].moved = true;
  m_tmp_board[move.from] = NoPiece;

  // Get the King position
  // FIXME: Optim memoriser la position du roi au lieu de la retrouver
  for (king = 0u; king < NbSquares; ++king)
    {
      if ((m_tmp_board[king].type == PieceType::King) &&
          (m_tmp_board[king].color == side))
        {
          //std::cout << "Le roi est en " << (int) king << std::endl;
          break;
        }
    }

  //
  Color xside = opposite(side);
  if (!attack(m_tmp_board, king, xside))
    {
      m_legal_moves.push_back(move);
      return true;
    }
  else
    {
      //std::cout << "Le coup laisse le roi en echec" << std::endl;
      return false;
    }
}

// FIXME: Optim memoriser la position du roi au lieu de la retrouver
bool Rules::inCheck(const Color side) const
{
  // Special case for Neural network using empty chessboard with no Kings
  if (hasNoKing)
    return false;

  for (uint8_t i = 0u; i < NbSquares; ++i)
    {
      if ((m_board[i].type == PieceType::King) &&
          (m_board[i].color == side))
        {
          Color xside = opposite(side);
          return attack(m_board, i, xside);
        }
    }
  assert(1 && "inCheck() did not find the King");
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
                  if (n == -1)
                    break;

                  if (n == sq)
                    return true;

                  if (position[n].type != PieceType::Empty)
                    break;

                  if (!c_can_slide[piece])
                    break;
                }
            }
        }
    }
  return false;
}

void Rules::dispLegalMoves() const
{
  std::cout << "Legal moves: " << std::endl;
  for (const auto it: m_legal_moves)
    {
      std::cout << "  " << it << std::endl;
    }
  std::cout << std::endl;
}

Status Rules::generateValidMoves()
{
  PieceType pt;
  int n;
  Piece p;

  // if (Status::Playing != m_status)
  //  return m_status;

  m_legal_moves.clear();
  for (uint8_t ij = 0u; ij < NbSquares; ++ij)
    {
      // Ignore: empty position and pieces of the opposite side
      p = m_board[ij];
      if ((p.type == PieceType::Empty) || (p.color != m_side))
        continue ;

      pt = static_cast<PieceType>(p.type);
      if (pt != PieceType::Pawn)
        {
          for (uint8_t m = 0u; m < c_relative_movements[pt].size(); ++m)
            {
              for (n = ij;;)
                {
                  n = c_mailbox120[c_mailbox64[n] + c_relative_movements[pt][m]];

                  // Outside the chessboard
                  if (n == -1)
                    break;

                  p = m_board[n];
                  if (p.type != PieceType::Empty)
                    {
                      if (p.color != m_side)
                        {
                          tryMove(Move(ij, n), m_side);
                        }
                      break;
                    }
                  tryMove(Move(ij, n), m_side);
                  if (c_can_slide[m_board[ij].type] == false)
                    break;
                }
            }
        }
      else // Pawns
        {
          if (m_side == Color::White)
            {
              // Take opponent piece
              if (COL(ij) != 0 && m_board[ij - 9u].type != PieceType::Empty && m_board[ij - 9u].color != Color::White)
                tryMove(Move(ij, ij - 9u), m_side);

              // Take opponent piece
              if (COL(ij) != 7u && m_board[ij - 7u].type != PieceType::Empty && m_board[ij - 7u].color != Color::White)
                tryMove(Move(ij, ij - 7u), m_side);

              if (m_board[ij - 8u].type == PieceType::Empty)
                {
                  // Move one square
                  tryMove(Move(ij, ij - 8u), m_side);

                  // Move two squares
                  if (ij >= sqA2 && m_board[ij - 16u].type == PieceType::Empty)
                    tryMove(Move(ij, ij - 16u), m_side);
                }
            }
          else
            {
              // Take opponent piece
              if (COL(ij) != 7u && m_board[ij + 9u].type != PieceType::Empty && m_board[ij + 9u].color != Color::Black)
                tryMove(Move(ij, ij + 9u), m_side);

              // Take opponent piece
              if (COL(ij) != 0u && m_board[ij + 7u].type != PieceType::Empty && m_board[ij + 7u].color != Color::Black)
                tryMove(Move(ij, ij + 7u), m_side);

              if (m_board[ij + 8u].type == PieceType::Empty)
                {
                  // Move one square
                  tryMove(Move(ij, ij + 8u), m_side);

                  // Move two squares
                  if (ij <= sqH7 && m_board[ij + 16u].type == PieceType::Empty)
                    tryMove(Move(ij, ij + 16u), m_side);
                }
            }
        }
    }

  // Generate White castle move
  if (m_side == Color::White)
    {
      // King status
      p = m_board[sqE1];
      if ((p.type == PieceType::King) && (p.color == Color::White) && (!p.moved) && (!inCheck(Color::White)))
        {
          // Rook H1 status for little castle
          p = m_board[sqH1];
          if ((p.type == PieceType::Rook) && (p.color == Color::White) && (!p.moved))
            {
              // Squares F1 and G1 status
              if ((m_board[sqF1].type == PieceType::Empty) && (!attack(m_board, sqF1, Color::Black)) &&
                  (m_board[sqG1].type == PieceType::Empty))
                {
                  // tryMove will test if G1 is not in check
                  tryMove(Move(sqE1, sqG1), m_side);
                }
            }

          // Rook A1 status for big castle
          p = m_board[sqA1];
          if ((p.type == PieceType::Rook) && (p.color == Color::White) && (!p.moved))
            {
              // Squares B1, C1 and D1 status
              if ((m_board[sqD1].type == PieceType::Empty) && (!attack(m_board, sqD1, Color::Black)) &&
                  (m_board[sqB1].type == PieceType::Empty) && (m_board[sqC1].type == PieceType::Empty))
                {
                  // Allowed if C1 is not in check
                  tryMove(Move(sqE1, sqC1), m_side);
                }
            }
        }
    }
  else // Generate Black castle move
    {
      // King status
      p = m_board[sqE8];
      if ((p.type == PieceType::King) && (p.color == Color::Black) && (!p.moved) && (!inCheck(Color::Black)))
        {
          // Rook H8 status for little castle
          p = m_board[sqH8];
          if ((p.type == PieceType::Rook) && (p.color == Color::Black) && (!p.moved))
            {
              // Rook F8 and G8 squares status
              if ((m_board[sqF8].type == PieceType::Empty) && (!attack(m_board, sqF8, Color::White)) &&
                  (m_board[sqG8].type == PieceType::Empty))
                {
                  // Allowed if G1 is not in check
                  tryMove(Move(sqE8, sqG8), m_side);
                }
            }

          // Rook A8 status for big castle
          p = m_board[sqA8];
          if ((p.type == PieceType::Rook) && (p.color == Color::Black) && (!p.moved))
            {
              // Squares B8, C8 and D8 status
              if ((m_board[sqD8].type == PieceType::Empty) && (!attack(m_board, sqD8, Color::White)) &&
                  (m_board[sqB8].type == PieceType::Empty) && (m_board[sqC8].type == PieceType::Empty))
                {
                  // Allowed if C8 is not in check
                  tryMove(Move(sqE8, sqC8), m_side);
                }
            }
        }
    }
  m_status = getStatus();
  return m_status;
}

// generateValidMoves() shall be called before
Status Rules::getStatus() const
{
  if (m_legal_moves.size() != 0)
    return Status::Playing;

  bool check = inCheck(m_side);
  if ((check) && (Color::White == m_side))
    return Status::BlackWon;

  if ((check) && (Color::Black == m_side))
    return Status::WhiteWon;

  // Hack: If there are no Kings on chessboard
  // (for neural training or unit test) this
  // error will be returned instead of Pat.
  if (hasNoKing)
    return Status::NoMoveAvailable;

  return Status::Stalemate;
}

// generateValidMoves() shall be called before
// FIXME: comparer le champ promotion ???
bool Rules::isValidMove(const Move move) const
{
  assert(m_legal_moves.size() != 0);
  for (auto it: m_legal_moves)
    {
      if ((it.from == move.from) && (it.to == move.to))
        return true;
    }
  return false;
}
