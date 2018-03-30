#ifndef FEN_HPP
#define FEN_HPP

#  include "Chess/Board.hpp"

// https://lichess.org

//! \brief Load a chessboard from a string using the Forsyth-Edwards Notation
bool load(std::string const& fen, chessboard& board, Color &color, int& en_passant);

//! \brief Save a chessboard with the Forsyth-Edwards Notation
std::string save(const chessboard& board);

#endif
