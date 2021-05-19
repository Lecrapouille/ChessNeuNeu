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

#ifndef TSCP_HPP
#  define TSCP_HPP

#  include "Player.hpp"
#  include "Utils/IPC.hpp"

// *****************************************************************************
//! \brief Implement a chess player. Call the TSCP program and communicate with
//! it through a bidirectional pipe.
// *****************************************************************************
class Tscp: public IPC, public IPlayer
{
public:

    Tscp(const Rules &rules, const Color side);
    ~Tscp();
    virtual std::string play() override;
    virtual void abort() override;

private:

    //! \brief We need to access to the chess rules for getting the list of
    //! played moves.
    const Rules &m_rules;
    bool m_aborting = false;
};

#endif
