//=====================================================================
// ChessNeuNeu: Non serious chess engine for learning neural networks.
// Copyright 2018 -- 2022 Quentin Quadrat <lecrapouille@gmail.com>
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

#include "Player.hpp"
#include <algorithm>

//------------------------------------------------------------------------------
static const char *c_player_types[] =
{
    [PlayerType::HumanPlayer] = "Human",
    [PlayerType::StockfishIA] = "Stockfish",
    [PlayerType::TscpIA] = "TSCP",
    [PlayerType::LokiIA] = "Loki",
    [PlayerType::NeuNeuIA] = "NeuNeu"
};

//------------------------------------------------------------------------------
const char *playerType(const PlayerType p)
{
    return c_player_types[p];
}

//------------------------------------------------------------------------------
PlayerType playerType(const std::string& player)
{
    std::string name(player);
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);
    if (name == "stockfish") return PlayerType::StockfishIA;
    if (name == "neuneu") return PlayerType::NeuNeuIA;
    if (name == "human") return PlayerType::HumanPlayer;
    if (name == "tscp") return PlayerType::TscpIA;
    if (name == "loki") return PlayerType::LokiIA;

    throw std::string("Unknonw PlayerType '" + player + "'");
}

//------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const PlayerType& p)
{
    os << c_player_types[p];
    return os;
}
