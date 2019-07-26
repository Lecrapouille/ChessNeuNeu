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

bool Rules::import(std::string const& fen)
{
  uint32_t count_rows = 0u;
  uint32_t count_cols = 0u;
  uint32_t i = 0u;
  uint32_t ij = 0u;
  uint32_t kings[2] = { 0u, 0u };

  m_side = Color::White;

  m_castle[Color::White] = Castle::NoCastle;
  m_castle[Color::Black] = Castle::NoCastle;

  while ((fen[i] != '\0') && (fen[i] != ' '))
    {
      if (fen[i] == '/')
        {
          // End of chessboard rows
          if (++count_rows >= NbRows) goto l_err_rows1;
          if (count_cols < NbCols) goto l_err_cols1;
          count_cols = 0u;
        }
      else if ((fen[i] >= '1') && (fen[i] <= '8'))
        {
          // Parse empty squares
          uint32_t skip_squares = static_cast<uint32_t>(fen[i] - '0');
          count_cols += skip_squares;
          if (count_cols > NbCols) goto l_err_cols2;
          while (skip_squares--)
            m_board[ij++] = NoPiece;
        }
      else
        {
          // Parse pieces and count kings
          Piece p = char2Piece(fen[i]);
          if (p == NoPiece) goto l_err_piece;
          if (p.type() == PieceType::King) kings[p.color()] += 1u;
          if (++count_cols > NbCols) goto l_err_cols2;
          m_board[ij++] = p;
        }
      ++i;
    }

  if (ij < NbSquares) goto l_err_rows2;

  // Expected 0 or 1 king by side
  if ((kings[Color::White] > 1) || (kings[Color::Black] != kings[Color::White]))
    goto l_err_kings;
  m_no_kings = (0u == kings[Color::White]);

  // Parse color
  if (fen[i] == ' ') ++i; else goto l_err_space;
  if ((fen[i] != 'w') && (fen[i] != 'b')) goto l_err_color;
  m_side = (fen[i++] == 'b') ? Color::Black : Color::White;

  // Parse possible castles (max 4 characters)
  if (fen[i++] != ' ') goto l_err_space;
  if (fen[i] == '-') ++i; else
    {
      uint8_t c = 0;
      while (fen[i] != ' ')
        {
          if (c > 3) goto l_err_castle1;
          else if (fen[i] == 'K') m_castle[Color::Black] |= Castle::Little;
          else if (fen[i] == 'Q') m_castle[Color::Black] |= Castle::Big;
          else if (fen[i] == 'k') m_castle[Color::White] |= Castle::Little;
          else if (fen[i] == 'q') m_castle[Color::White] |= Castle::Big;
          else goto l_err_castle1;
          //TODO if (!verifyValidCastlingInfo()) goto l_err_castle2;
          ++i; ++c;
        }
    }

  // En passant
  if (fen[i++] != ' ') goto l_err_ep;
  if (fen[i] == '-') m_ep = Square::OOB;
  else if ((fen[i] >= 'a') && (fen[i] <= 'h') && (fen[i + 1u] >= '1') && (fen[i + 1u] <= '8'))
    m_ep = toSquare(&fen[i]);
  else goto l_err_ep;

  // Ignore other part (half move)
  // ++i;

  // Success
  return true;

  // Failures
l_err_rows1:
  std::cerr << "Bad FEN format: there are too many '/' separators: " << int(count_rows) << std::endl;
  return false;

l_err_rows2:
  std::cerr << "Bad FEN format: there are not enough '/' separators: " << int(count_rows) << std::endl;
  return false;

l_err_cols1:
  std::cerr << "Bad FEN format: there is less than 8 columns: " << int(count_cols) << std::endl;
  return false;

l_err_cols2:
  std::cerr << "Bad FEN format: there is more than 8 columns: " << int(count_cols) << std::endl;
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

l_err_kings:
  std::cerr << "Expecting 1 or 0 King by color" << std::endl;
  return false;

l_err_ep:
  std::cerr << "Bad FEN format: invalid en passant format '" << fen[i] << "'" << std::endl;
  return false;
}

/*std::string export(const chessboard& board)
{
  return "TODO";
  }*/
