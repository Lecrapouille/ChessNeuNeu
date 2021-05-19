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

#ifndef NEUNEU_HPP
#  define NEUNEU_HPP

#  include "Player.hpp"

//! \brief Special enum for neural network. Shall match enum of pieces.
enum NeuralPiece {
    NeuralEmpty     = PieceType::Empty, // Not used but only for matching with enum PieceType
    NeuralRook      = PieceType::Rook,
    NeuralKnight    = PieceType::Knight,
    NeuralBishop    = PieceType::Bishop,
    NeuralQueen     = PieceType::Queen,
    NeuralKing      = PieceType::King,
    NeuralWhitePawn = PieceType::WPawn, // Aka for PieceType::Pawn
    NeuralBlackPawn = PieceType::BPawn, // Aka for PieceType::NotUsed
};

//! \brief Print the type of Neural Network pieces.
std::ostream& operator<<(std::ostream& os, const NeuralPiece& p);

//! \brief Structure holding the neural network.
//!
//! The matrix is 64x64 because it holds the whole combinaison of chess
//! movements. Weights of the matrix makes allow or forbid movement. For example
//! for a pawn Synaps.weights[e2][e4] will be around 1.0f (allowed) and for a
//! rook Synaps.weights[e2][f3] will be around 0.0f (forbidden).
struct Synaps
{
    float weights[NbSquares][NbSquares];
};

// ***********************************************************************************************
//! \brief Implement a chess player. Here, we are protopying a hand made neural network
//! learning by itself how to move pieces (for the moment learnt from an empty square).
//! For more information see the pdf document in the doc/ folder.
// ***********************************************************************************************
class NeuNeu: public IPlayer
{
public:

    NeuNeu(const Rules &rules, const Color side);
    ~NeuNeu();

    //! \brief return the valid move when playing against a component.
    //!
    //! Random a piece to move then make the neural network computes the
    //! probablity for each destination. Random a move depending its probabilty to
    //! appear.
    //!
    //! \return the move as string (ie "e2e4" or "e7e8Q") or Move::none if or
    //! Move::error.
    virtual std::string play() override;

    //! \brief Abort signal for halting properly the play() method.
    //! Implement it as you desired (usually a simple bool).
    virtual void abort() override
    {
        // TODO
    }

private:

    //! \brief Cast a chessboard figure to a neural network figure enum.
    inline NeuralPiece Piece2NeuralPiece(const Piece piece) const
    {
        switch (piece.type)
        {
        case PieceType::Pawn:
            if (piece.color == Color::Black)
                return NeuralPiece::NeuralBlackPawn;
            return NeuralPiece::NeuralWhitePawn;
        case PieceType::Rook:
            return NeuralPiece::NeuralRook;
        case PieceType::Bishop:
            return NeuralPiece::NeuralBishop;
        case PieceType::Knight:
            return NeuralKnight;
        case PieceType::Queen:
            return NeuralPiece::NeuralQueen;
        case PieceType::King:
            return NeuralPiece::NeuralKing;
        case PieceType::Empty:
        default:
            return NeuralPiece::NeuralEmpty;
        }
    }

    //! \brief Cast the enum of network figure enum to a chessboard figure.
    inline Piece NeuralPiece2Piece(const NeuralPiece piece) const
    {
        switch (piece)
        {
        case NeuralPiece::NeuralBlackPawn:
            return BlackPawn;
        case NeuralPiece::NeuralWhitePawn:
            return WhitePawn;
        case NeuralPiece::NeuralRook:
            return WhiteRook;
        case NeuralPiece::NeuralBishop:
            return WhiteBishop;
        case NeuralPiece::NeuralKnight:
            return WhiteKnight;
        case NeuralPiece::NeuralQueen:
            return WhiteQueen;
        case NeuralPiece::NeuralKing:
            return WhiteKing;
        case NeuralPiece::NeuralEmpty:
        default:
            return NoPiece;
        }
    }

    //! \brief Do not allow placing pawns on row 1 or 8 when training them to
    //! displace. Indeed these rows are used when the pawn is promoted to a
    //! figure.
    inline bool isValidPawnPosition(const uint8_t from) const
    {
        return (from >= sqA7) && (from <= sqH2);
    }

    //! \brief Train the IA.
    void trainSynaps(Piece piece, Synaps &synaps);

    //! \brief Make synaps do a move
    uint8_t synapsPlay(const uint8_t from, Synaps &synaps);

    //! \brief Display on the console synaps of the neural network of the
    //! concerned figure.
    void showSynaps(const NeuralPiece piece);

    //! \brief Display probabilties of movements starting from origin \c from.
    void showProbabilities(const uint8_t from);

private:

    //! \brief Use Chess rules as supervizor.
    const Rules &m_rules;

    //! \brief Hold neural network for each figures.
    Synaps *m_neurons[8u];

    //! \brief Inputs of the neural network (1.0f means a figure and 0.0f means no
    //! figure).
    float e[NbSquares];

    //! \brief outputs of the neural network (probabilities of the movement).
    float q[NbSquares];
};

#endif
