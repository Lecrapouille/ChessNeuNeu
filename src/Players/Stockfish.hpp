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

#ifndef STOCKFISH_HPP
#  define STOCKFISH_HPP

#  include "Player.hpp"
#  include "Utils/IPC.hpp"

class Stockfish: public IPC, public IPlayer
{
public:

  Stockfish(const Rules &rules, const Color side, std::string const& fen);
  ~Stockfish();
  virtual std::string play() override;
  virtual void abort() override;
  void debug();

private:

  std::string m_fen;
  const Rules &m_rules;
  bool m_aborting = false;
};

#endif
