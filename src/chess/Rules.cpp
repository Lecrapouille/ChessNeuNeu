#include "Rules.hpp"
#include <array>
#include <valarray>
#include <cassert>

//! \brief Give a name to each chessboard squares.
//! Used For castle and pawn moves.
enum Square {
  sqA8, sqB8, sqC8, sqD8, sqE8, sqF8, sqG8, sqH8,
  sqA7, sqB7, sqC7, sqD7, sqE7, sqF7, sqG7, sqH7,
  sqA6, sqB6, sqC6, sqD6, sqE6, sqF6, sqG6, sqH6,
  sqA5, sqB5, sqC5, sqD5, sqE5, sqF5, sqG5, sqH5,
  sqA4, sqB4, sqC4, sqD4, sqE4, sqF4, sqG4, sqH4,
  sqA3, sqB3, sqC3, sqD3, sqE3, sqF3, sqG3, sqH3,
  sqA2, sqB2, sqC2, sqD2, sqE2, sqF2, sqG2, sqH2,
  sqA1, sqB1, sqC1, sqD1, sqE1, sqF1, sqG1, sqH1
};

//! \brief Store square names. Used for creating move notes.
static const std::array<const char[3], 64> c_mailboxToChar =
  {{
      "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
      "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
      "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
      "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
      "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
      "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
      "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
      "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
    }};

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

//! \brief Define for each pieces their displacement in the c_mailbox64.
//! Let's suppose a King is placed on the chessboard square e4. This
//! corresponds to the square 65 of the c_mailbox64. The King can moves to
//! squares d3, e3, f3, f4, f5, e5, d5 and d4 which corresponds to squares
//! 64, 75, 76, 66, 56, 55, 54 and 64 in c_mailbox64. Relative displacement
//! is their difference with the 65. For example -11 = 54 - 65. For other
//! pieces which can slide we store the displacement of a distance of 1.
static const std::valarray<int> c_relative_movements[8] =
  {
    [PieceType::Empty] = {},
    [PieceType::Rook] = { -10, -1, 1, 10 },
    [PieceType::Knight] = { -21, -19, -12, -8, 8, 12, 19, 21 },
    [PieceType::Bishop] = { -11, -9, 9, 11 },
    [PieceType::Queen] = { -11, -10, -9, -1, 1, 9, 10, 11 },
    [PieceType::King] = { -11, -10, -9, -1, 1, 9, 10, 11 },
    [PieceType::Pawn] = {},
    [PieceType::NotUsed] = {},
  };

//! \brief Define pieces which can do more than one c_relative_movements
//! in a single step.
static const std::array<bool, 8> c_can_slide =
  {{
      [PieceType::Empty] = false,
      [PieceType::Rook] = true,
      [PieceType::Knight] = false,
      [PieceType::Bishop] = true,
      [PieceType::Queen] = true,
      [PieceType::King] = false,
      [PieceType::Pawn] = false,
      [PieceType::NotUsed] = false,
    }};

std::ostream& operator<<(std::ostream& os, const Status& s)
{
  switch (s)
    {
    case Status::WhiteWon:
      os << "White won";
      break;
    case Status::BlackWon:
      os << "Black won";
      break;
    case Status::Pat:
      os << "Pat";
      break;
    case Status::Draw:
      os << "Draw";
      break;
    case Status::InternalError:
      os << "InternalError";
      break;
    default:
      os << "Playing";
      break;
    }
  return os;
}

std::ostream& operator<<(std::ostream& os, const Move& move)
{
  if (move.castle == Castle::Little)
    {
      os << "O-O";
    }
  else if (move.castle == Castle::Big)
    {
      os << "O-O-O";
    }
  else
    {
      os << c_mailboxToChar[move.from] << '-' << c_mailboxToChar[move.to];
    }

  if (move.ep)
    {
      os << "ep";
    }

  if (move.promote)
    {
      os << move.piece.type;
    }

  if (move.check)
    {
      os << "+";
    }

  return os;
}

std::ostream& operator<<(std::ostream& os, chessboard& position)
{
  Piece piece;

  os << " +---+---+---+---+---+---+---+---+" << std::endl;
  for (uint8_t ij = 0u; ij < 64u; ++ij)
    {
      piece = static_cast<Piece>(position[ij]);
      os << " | " << piece;

      if (0 == ((ij+1) & 7))
        {
          os << " |" << std::endl;
          os << " +---+---+---+---+---+---+---+---+" << std::endl;
        }
    }
  return os;
}

// FIXME Rules devrait etre Observer et envoyer un event a IBoard et Board est un IBoard en SFML
// alors que ConsolBoard affiche un echiquier et lit stdin
void Rules::loadPosition(const chessboard& board)
{
  m_current_position = board;
}

void Rules::loadPosition(const std::string& moves)
{
  m_current_position = c_init_board;

  for (uint32_t i = 0u; i < moves.length(); i += 5u)
    {
      makeMove(moves.substr(i, 4u));
    }
}

std::string Move2str(const uint8_t sq)
{
  return std::string(c_mailboxToChar[sq]);
}

std::string Move2str(const Move move)
{
  return std::string(c_mailboxToChar[move.from]) + std::string(c_mailboxToChar[move.to]);
}

std::string Move2str(const uint8_t from, const uint8_t to)
{
  return std::string(c_mailboxToChar[from]) + std::string(c_mailboxToChar[to]);
}

// FIXME: a placer dans Rules qui appelle Board::moveBack via un observer (ou meme pas: directement)
// 5 is the string size for example "e2e4 "
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

      std::cout << m_current_position << std::endl;
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
  m_current_position[m2] = m_current_position[m1];
  m_current_position[m2].moved = true;
  m_current_position[m1] = EmptySquare;

  // Add the note move to moved notes
  m_moved += next_move;
  m_moved += ' ';

  // Switch player
  std::cout << m_side << " played " << next_move << std::endl;
  sidePlayed();
  std::cout << m_current_position << std::endl;
  std::cout << m_side << " are playing" << std::endl;
  generateValidMoves();
  dispLegalMoves();
}

bool Rules::tryMove(const Move move, const Color side)
{
  uint8_t king;

  // Simulate the next move.
  // FIXME: code pas fini (manque promotion, en passant)
  // et en double avec makeMove (mais sur un echiquier temporaire)
  m_next_position = m_current_position;
  m_next_position[move.to] = m_next_position[move.from];
  m_next_position[move.to].moved = true;
  m_next_position[move.from] = EmptySquare;

  // Get the King position
  // FIXME: Optim memoriser la position du roi au lieu de la retrouver
  for (king = 0u; king < 64u; ++king)
    {
      if ((m_next_position[king].type == PieceType::King) &&
          (m_next_position[king].color == side))
        {
          //std::cout << "Le roi est en " << (int) king << std::endl;
          break;
        }
    }

  //
  Color xside = opposite(side);
  if (!attack(m_next_position, king, xside))
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

  for (uint8_t i = 0u; i < 64u; ++i)
    {
      if ((m_current_position[i].type == PieceType::King) &&
          (m_current_position[i].color == side))
        {
          std::cout << "inCheck Roi: " << (int) i << std::endl;
          Color xside = opposite(side);
          return attack(m_current_position, i, xside);
        }
    }
  assert(1 && "inCheck() did not find the King");
  return false;
}

bool Rules::attack(const std::array<Piece, 64>& position, const uint8_t sq, const Color side) const
{
  int n;

  for (uint8_t i = 0u; i < 64u; ++i)
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
  for (uint8_t ij = 0u; ij < 64u; ++ij)
    {
      // Ignore: empty position and pieces of the opposite side
      p = m_current_position[ij];
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

                  p = m_current_position[n];
                  if (p.type != PieceType::Empty)
                    {
                      if (p.color != m_side)
                        {
                          tryMove(Move(ij, n), m_side);
                        }
                      break;
                    }
                  tryMove(Move(ij, n), m_side);
                  if (c_can_slide[m_current_position[ij].type] == false)
                    break;
                }
            }
        }
      else // Pawns
        {
          if (m_side == Color::White)
            {
              // Take opponent piece
              if (COL(ij) != 0 && m_current_position[ij - 9u].type != PieceType::Empty && m_current_position[ij - 9u].color != Color::White)
                tryMove(Move(ij, ij - 9u), m_side);

              // Take opponent piece
              if (COL(ij) != 7u && m_current_position[ij - 7u].type != PieceType::Empty && m_current_position[ij - 7u].color != Color::White)
                tryMove(Move(ij, ij - 7u), m_side);

              if (m_current_position[ij - 8u].type == PieceType::Empty)
                {
                  // Move one square
                  tryMove(Move(ij, ij - 8u), m_side);

                  // Move two squares
                  if (ij >= sqA2 && m_current_position[ij - 16u].type == PieceType::Empty)
                    tryMove(Move(ij, ij - 16u), m_side);
                }
            }
          else
            {
              // Take opponent piece
              if (COL(ij) != 7u && m_current_position[ij + 9u].type != PieceType::Empty && m_current_position[ij + 9u].color != Color::Black)
                tryMove(Move(ij, ij + 9u), m_side);

              // Take opponent piece
              if (COL(ij) != 0u && m_current_position[ij + 7u].type != PieceType::Empty && m_current_position[ij + 7u].color != Color::Black)
                tryMove(Move(ij, ij + 7u), m_side);

              if (m_current_position[ij + 8u].type == PieceType::Empty)
                {
                  // Move one square
                  tryMove(Move(ij, ij + 8u), m_side);

                  // Move two squares
                  if (ij <= sqH7 && m_current_position[ij + 16u].type == PieceType::Empty)
                    tryMove(Move(ij, ij + 16u), m_side);
                }
            }
        }
    }

  // Generate White castle move
  if (m_side == Color::White)
    {
      // King status
      p = m_current_position[sqE1];
      if ((p.type == PieceType::King) && (p.color == Color::White) && (!p.moved) && (!inCheck(Color::White)))
        {
          // Rook H1 status for little castle
          p = m_current_position[sqH1];
          if ((p.type == PieceType::Rook) && (p.color == Color::White) && (!p.moved))
            {
              // Squares F1 and G1 status
              if ((m_current_position[sqF1].type == PieceType::Empty) && (!attack(m_current_position, sqF1, Color::Black)) &&
                  (m_current_position[sqG1].type == PieceType::Empty))
                {
                  // tryMove will test if G1 is not in check
                  tryMove(Move(sqE1, sqG1), m_side);
                }
            }

          // Rook A1 status for big castle
          p = m_current_position[sqA1];
          if ((p.type == PieceType::Rook) && (p.color == Color::White) && (!p.moved))
            {
              // Squares B1, C1 and D1 status
              if ((m_current_position[sqD1].type == PieceType::Empty) && (!attack(m_current_position, sqD1, Color::Black)) &&
                  (m_current_position[sqB1].type == PieceType::Empty) && (m_current_position[sqC1].type == PieceType::Empty))
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
      p = m_current_position[sqE8];
      if ((p.type == PieceType::King) && (p.color == Color::Black) && (!p.moved) && (!inCheck(Color::Black)))
        {
          // Rook H8 status for little castle
          p = m_current_position[sqH8];
          if ((p.type == PieceType::Rook) && (p.color == Color::Black) && (!p.moved))
            {
              // Rook F8 and G8 squares status
              if ((m_current_position[sqF8].type == PieceType::Empty) && (!attack(m_current_position, sqF8, Color::White)) &&
                  (m_current_position[sqG8].type == PieceType::Empty))
                {
                  // Allowed if G1 is not in check
                  tryMove(Move(sqE8, sqG8), m_side);
                }
            }

          // Rook A8 status for big castle
          p = m_current_position[sqA8];
          if ((p.type == PieceType::Rook) && (p.color == Color::Black) && (!p.moved))
            {
              // Squares B8, C8 and D8 status
              if ((m_current_position[sqD8].type == PieceType::Empty) && (!attack(m_current_position, sqD8, Color::White)) &&
                  (m_current_position[sqB8].type == PieceType::Empty) && (m_current_position[sqC8].type == PieceType::Empty))
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

  return Status::Pat;
}

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
