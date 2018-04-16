#include "Chess/Rules.hpp"

//! \brief Store piece names (white side). Used for drawing in the console a chessboard.
static const std::array<char, 8> c_white_piece_char =
  {{
      [PieceType::Empty] = ' ',
      [PieceType::Rook] = 'R',
      [PieceType::Knight] = 'N',
      [PieceType::Bishop] = 'B',
      [PieceType::Queen] = 'Q',
      [PieceType::King] = 'K',
      [PieceType::Pawn] = 'P',
      [PieceType::NotUsed] = ' ',
    }};

//! \brief Store piece names (black side). Used for drawing in the console a chessboard.
static const std::array<char, 8> c_black_piece_char =
  {{
      [PieceType::Empty] = ' ',
      [PieceType::Rook] = 'r',
      [PieceType::Knight] = 'n',
      [PieceType::Bishop] = 'b',
      [PieceType::Queen] = 'q',
      [PieceType::King] = 'k',
      [PieceType::Pawn] = 'p',
      [PieceType::NotUsed] = ' ',
    }};

//! \brief Associative table between an ascii code and a piece.
//! Used for promotion from a chess note. Used for parsing a FEN
//! string and storing piece in the chessboard.
static const std::map<char, Piece> c_char2piece =
  {
    // White Color
    { 'B', WhiteBishop }, { 'K', WhiteKing }, { 'N', WhiteKnight },
    { 'P', WhitePawn },   { 'Q', WhiteQueen}, { 'R', WhiteRook },
    // Black Color
    { 'b', BlackBishop }, { 'k', BlackKing }, { 'n', BlackKnight },
    { 'p', BlackPawn },   { 'q', BlackQueen}, { 'r', BlackRook },
  };

Piece char2Piece(const char c)
{
  auto it = c_char2piece.find(c);
  if (c_char2piece.end() != it)
    {
      return it->second;
    }
  return NoPiece;
}

char piece2char(const Piece p)
{
  return c_black_piece_char[p.type];
}

char piece2char(const PieceType p)
{
  return c_black_piece_char[p];
}

//! \brief Print the piece color.
std::ostream& operator<<(std::ostream& os, const Color& c)
{
  os << ((Color::White == c) ? "White" : "Black");
  return os;
}

//! \brief Print the type of piece.
std::ostream& operator<<(std::ostream& os, const PieceType& p)
{
  os << c_white_piece_char[p];
  return os;
}

//! \brief Print the type of piece.
// TODO: print other info (castle, moved, ...)
std::ostream& operator<<(std::ostream& os, const Piece& p)
{
  if (p.color == Color::White)
    {
      os << c_white_piece_char[p.type];
    }
  else
    {
      os << c_black_piece_char[p.type];
    }
  return os;
}

std::ostream& operator<<(std::ostream& os, chessboard& position)
{
  Piece piece;

  os << "     a   b   c   d   e   f   g   h  " << std::endl << std::endl;
  os << "   +---+---+---+---+---+---+---+---+" << std::endl;
  for (uint8_t ij = 0u; ij < NbSquares; ++ij)
    {
      if (0 == (ij & 7))
        {
          os << (int) (8u - ij / 8u) << " ";
        }
      piece = static_cast<Piece>(position[ij]);
      os << " | " << piece;

      if (0 == ((ij+1) & 7))
        {
          os << " |  " << (int) (8u - ij / 8u) << std::endl;
          os << "   +---+---+---+---+---+---+---+---+" << std::endl;
        }
    }
  os << std::endl << "     A   B   C   D   E   F   G   H  " << std::endl;
  return os;
}

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
    case Status::Stalemate:
      os << "Stalemate";
      break;
    case Status::InternalError:
      os << "Internal error";
      break;
    default:
      os << "Playing";
      break;
    }
  return os;
}

std::ostream& operator<<(std::ostream& os, const Move& move)
{
  if (move.castle & Castle::Little)
    {
      os << "O-O";
    }
  else if (move.castle & Castle::Big)
    {
      os << "O-O-O";
    }
  else
    {
      // FIXME: afficher piece
      os << c_square_names[move.from] << '-' << c_square_names[move.to];
    }

  if (move.ep)
    {
      os << "ep";
    }
  else if (move.promote != PieceType::Empty)
    {
      os << ':' << c_black_piece_char[move.promote];
    }

  if (move.check)
    {
      os << "+";
    }

  return os;
}
