#include "Chess/FEN.hpp"

// TODO check if Pawns are not in 8 or 1
// TODO check if Black move and White king is in check
// TODO Count max pieces > initial nb pieces + 8 pawns

// TODO
// si (aucun roque) alors marque le roi a moved
// sinon si petit roque impossible chercher la tour !A1 si elle existe et la mettre a moved
// sinon si grand roque impossible chercher la tour !H1 si elle existe et la mettre a moved
/*static bool verifyValidCastlingInfo(chessboard const& board, Color const side, bool little, bool big)
{
  if (! (little || big))
    return true;

  // TODO: les cas faux: retourner un message d'erreur
  if (Color::White == side)
    {
      if (WhiteKing != board[sqE1])
        return false;
      if (little && (WhiteRook != board[sqH1]))
        return false;
      if (big && (WhiteRook != board[sqA1]))
        return false;
    }
  else
    {
      if (BlackKing != board[sqE8])
        return false;
      if (little && (BlackRook != board[sqH8]))
        return false;
      if (big && (BlackRook != board[sqA8]))
        return false;
    }

  return true;
  }*/

 //FIXME

 /*bool load(std::string const& fen, Rules& rules)
{
  return load(fen, rules.m_board, rules.side, rules.en_passant, rules.hasKing);
  }*/

bool load(std::string const& fen, chessboard& board, Color &color, int& en_passant)
{
  uint8_t count_rows = 0u;
  uint8_t count_cols = 0u;
  uint8_t i = 0u;
  uint8_t ij = 0u;
  color = Color::White;

  // Parse Pieces
  while ((fen[i] != '\0') && (fen[i] != ' '))
    {
      if (fen[i] == '/')
        {
          if (++count_rows > 7) goto l_err_rows1;
          if (count_cols < 8) goto l_err_cols1;
          count_cols = 0u;
        }
      else if ((fen[i] >= '1') && (fen[i] <= '8'))
        {
          uint8_t skip_squares = fen[i] - '0';
          count_cols += skip_squares;
          if (count_cols > 8) goto l_err_cols2;
          for (uint8_t j = 0; j < skip_squares; ++j)
            board[ij++] = NoPiece;
        }
      else // pieces
        {
          Piece p = char2Piece(fen[i]);
          if (p == NoPiece) goto l_err_piece;
          if (++count_cols > 8) goto l_err_cols2;
          board[ij++] = p;
        }
      ++i;
    }

  if (ij < NbSquares) goto l_err_rows2;

  // Parse color
  if (fen[i] == ' ') ++i; else goto l_err_space;
  if ((fen[i] != 'w') && (fen[i] != 'b')) goto l_err_color;
  color = (fen[i++] == 'b') ? Color::Black : Color::White;

  // Parse possible castles (max 4 characters)
  if (fen[i] == ' ') ++i; else goto l_err_space;
  if (fen[i] == '-') ++i; else
    {
      uint8_t c = 0;
      while (fen[i] != ' ')
        {
          if (c > 3) goto l_err_castle1;
          else if (fen[i] == 'k') {} // TODO can_castle[Color::Black] = LitteCastle;
          else if (fen[i] == 'q') {} // TODO can_castle[Color::Black] = BigCastle;
          else if (fen[i] == 'K') {} // TODO can_castle[Color::White] = LitteCastle;
          else if (fen[i] == 'Q') {} // TODO can_castle[Color::White] = BigCastle;
          else goto l_err_castle1;
          //TODO if (!verifyValidCastlingInfo()) goto l_err_castle2;
          ++i; ++c;
        }
    }

  // En passant
  if (fen[i++] != ' ') goto l_err_ep;
  if (fen[i] == '-') en_passant = -1;
  else if ((fen[i] >= 'a') && (fen[i] <= 'h') && (fen[i + 1] >= '1') && (fen[i + 1] <= '8'))
    en_passant = toSquare(&fen[i]);
  else goto l_err_ep;

  // Ignore other part (half move)
  // ++i;

  // Success
  return true;

  // Failures
l_err_rows1:
  std::cerr << "Bad FEN format: there are too many '/' separators: " << (int) count_rows << std::endl;
  return false;

l_err_rows2:
  std::cerr << "Bad FEN format: there are not enough '/' separators: " << (int) count_rows << std::endl;
  return false;

l_err_cols1:
  std::cerr << "Bad FEN format: there is less than 8 columns: " << (int) count_cols << std::endl;
  return false;

l_err_cols2:
  std::cerr << "Bad FEN format: there is more than 8 columns: " << (int) count_cols << std::endl;
  return false;

l_err_piece:
  std::cerr << "Bad FEN format: invalid piece format '" << fen[i] << "'" << std::endl;
  return false;

l_err_space:
  std::cerr << "Bad FEN format: missing space char. Got instead '" << fen[i] << "' char" << std::endl;
  return false;

l_err_color:
  std::cerr << "Bad FEN format: invalid color format '" << fen[i] << "'" << std::endl;
  return false;

l_err_castle1:
  std::cerr << "Bad FEN format: invalid castle format '" << fen[i] << "'" << std::endl;
  return false;

  /*l_err_castle2:
  std::cerr << "Bad FEN format: incompatible castle" << std::endl;
  return false;*/

l_err_ep:
  std::cerr << "Bad FEN format: invalid en passant format '" << fen[i] << "'" << std::endl;
  return false;
}
