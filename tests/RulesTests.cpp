#include "Chess/Rules.hpp"
#include "Chess/FEN.hpp"
#include <crpcut.hpp>
#include <iostream>
#include <ostream>
#include <algorithm>

TESTSUITE(PawnMoves)
{
  TEST(NoMove)
  {
    // Empty chessboard. No Kings.
    Rules rules(Color::White, Chessboard::Empty, WithNoKings);

    // Check no longer moves.
    ASSERT_EQ(Status::NoMoveAvailable, rules.generateValidMoves());
    ASSERT_EQ(0, rules.m_legal_moves.size());
  }

  TEST(PawnMoveWE2_BE7)
  {
    // Empty chessboard, No King, White Pawn E2
    Rules rules(Color::White, Chessboard::Empty, WithNoKings);
    rules.m_board[sqE2] = WhitePawn;
    rules.m_board[sqE7] = BlackPawn;

    // Check legal moves
    ASSERT_EQ(Status::Playing, rules.generateValidMoves());
    ASSERT_EQ(2, rules.m_legal_moves.size());
    std::vector<Move>::iterator b = rules.m_legal_moves.begin();
    std::vector<Move>::iterator e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("e2e3")));
    ASSERT_NE(e, std::find(b, e, Move("e2e4")));

    rules.makeMove("e2e3");

    ASSERT_EQ(Status::Playing, rules.generateValidMoves());
    ASSERT_EQ(2, rules.m_legal_moves.size());
    b = rules.m_legal_moves.begin();
    e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("e7e6")));
    ASSERT_NE(e, std::find(b, e, Move("e7e5")));
  }

  TEST(PawnMoveWD3_BD6)
  {
    // Empty chessboard, No King, White Pawn D3
    Rules rules(Color::White, Chessboard::Empty, WithNoKings);
    rules.m_board[sqD3] = WhitePawn;
    rules.m_board[sqD6] = BlackPawn;

    // Check legal moves
    ASSERT_EQ(Status::Playing, rules.generateValidMoves());
    ASSERT_EQ(1, rules.m_legal_moves.size());
    std::vector<Move>::iterator b = rules.m_legal_moves.begin();
    std::vector<Move>::iterator e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("d3d4")));

    rules.makeMove("e2e3");

    ASSERT_EQ(Status::Playing, rules.generateValidMoves());
    ASSERT_EQ(1, rules.m_legal_moves.size());
    b = rules.m_legal_moves.begin();
    e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("d6d5")));
  }

  TEST(PawnMoveWA4_BA5)
  {
    // Empty chessboard, No King, White Pawn A4
    Rules rules(Color::White, Chessboard::Empty, WithNoKings);
    rules.m_board[sqA4] = WhitePawn;
    rules.m_board[sqA5] = BlackPawn;

    // Check no longer moves.
    ASSERT_EQ(Status::NoMoveAvailable, rules.generateValidMoves());
    ASSERT_EQ(0, rules.m_legal_moves.size());
  }

  TEST(PawnMoveWA4_BB5)
  {
    // Empty chessboard, No King, White Pawn A4
    Rules rules(Color::White, Chessboard::Empty, WithNoKings);
    rules.m_board[sqA4] = WhitePawn;
    rules.m_board[sqB5] = BlackPawn;

    // Check legal moves
    ASSERT_EQ(Status::Playing, rules.generateValidMoves());
    ASSERT_EQ(2, rules.m_legal_moves.size());
    std::vector<Move>::iterator b = rules.m_legal_moves.begin();
    std::vector<Move>::iterator e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("a4a5")));
    ASSERT_NE(e, std::find(b, e, Move("a4b5")));

    rules.m_side = Color::Black;
    ASSERT_EQ(Status::Playing, rules.generateValidMoves());
    ASSERT_EQ(2, rules.m_legal_moves.size());
    b = rules.m_legal_moves.begin();
    e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("b5a4")));
    ASSERT_NE(e, std::find(b, e, Move("b5b4")));
  }

  TEST(PawnPromotionMove)
  {
    // Empty chessboard, No King, White Pawn G7
    Rules rules(Color::White, Chessboard::Empty, WithNoKings);
    rules.m_board[sqG7] = WhitePawn;
    rules.m_board[sqB2] = BlackPawn;

    // Check legal moves
    ASSERT_EQ(Status::Playing, rules.generateValidMoves());
    ASSERT_EQ(4, rules.m_legal_moves.size());
    std::vector<Move>::iterator b = rules.m_legal_moves.begin();
    std::vector<Move>::iterator e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("g7g8R")));
    ASSERT_NE(e, std::find(b, e, Move("g7g8N")));
    ASSERT_NE(e, std::find(b, e, Move("g7g8B")));
    ASSERT_NE(e, std::find(b, e, Move("g7g8Q")));

    rules.makeMove("g7g8B");
    ASSERT_EQ(true, WhiteBishop == rules.m_board[sqG8]);

    ASSERT_EQ(Status::Playing, rules.generateValidMoves());
    ASSERT_EQ(4, rules.m_legal_moves.size());
    b = rules.m_legal_moves.begin();
    e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("b2b1R")));
    ASSERT_NE(e, std::find(b, e, Move("b2b1N")));
    ASSERT_NE(e, std::find(b, e, Move("b2b1B")));
    ASSERT_NE(e, std::find(b, e, Move("b2b1Q")));

    rules.makeMove("b2b1R");
    ASSERT_EQ(BlackRook, rules.m_board[sqB1]);
  }

  TEST(EnPassantAllowed)
  {
    chessboard board(Chessboard::Init);
    Color color;
    int ep;

    ASSERT_EQ(true, load("8/8/8/8/3pP3/8/8/8 b - e3", board, color, ep));
    ASSERT_EQ(Color::Black, color);
    ASSERT_EQ(sqE3, ep);
    Rules rules(color, board, WithNoKings);
    ASSERT_EQ(2, rules.m_legal_moves.size());
    std::vector<Move>::iterator b = rules.m_legal_moves.begin();
    std::vector<Move>::iterator e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("d4d3")));
    ASSERT_NE(e, std::find(b, e, Move("d4e3")));
  }

  TEST(EnPassantNotAllowed)
  {
    chessboard board(Chessboard::Init);
    Color color;
    int ep;

    ASSERT_EQ(true, load("8/8/8/8/3pP3/8/8/8 b - -", board, color, ep));
    ASSERT_EQ(Color::Black, color);
    ASSERT_EQ(-1, ep);
    Rules rules(color, board, WithNoKings);
    ASSERT_EQ(1, rules.m_legal_moves.size());
    std::vector<Move>::iterator b = rules.m_legal_moves.begin();
    std::vector<Move>::iterator e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("d4d3")));
  }
}

TESTSUITE(KnightMoves)
{
  TEST(BasicKnightMoves)
  {
    // Empty chessboard, No King, White Knight B1, Black Knight H8
    Rules rules(Color::White, Chessboard::Empty, WithNoKings);
    rules.m_board[sqB1] = WhiteKnight;
    rules.m_board[sqH8] = BlackKnight;

    // Check legal moves
    ASSERT_EQ(Status::Playing, rules.generateValidMoves());
    ASSERT_EQ(3, rules.m_legal_moves.size());
    std::vector<Move>::iterator b = rules.m_legal_moves.begin();
    std::vector<Move>::iterator e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("b1a3")));
    ASSERT_NE(e, std::find(b, e, Move("b1c3")));
    ASSERT_NE(e, std::find(b, e, Move("b1d2")));

    rules.makeMove("b1c3");

    ASSERT_EQ(Status::Playing, rules.generateValidMoves());
    ASSERT_EQ(2, rules.m_legal_moves.size());
    b = rules.m_legal_moves.begin();
    e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("h8f7")));
    ASSERT_NE(e, std::find(b, e, Move("h8g6")));

    rules.makeMove("h8f7");

    ASSERT_EQ(Status::Playing, rules.generateValidMoves());
    ASSERT_EQ(8, rules.m_legal_moves.size());
    b = rules.m_legal_moves.begin();
    e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("c3b1")));
    ASSERT_NE(e, std::find(b, e, Move("c3a2")));
    ASSERT_NE(e, std::find(b, e, Move("c3a4")));
    ASSERT_NE(e, std::find(b, e, Move("c3b5")));
    ASSERT_NE(e, std::find(b, e, Move("c3d5")));
    ASSERT_NE(e, std::find(b, e, Move("c3e4")));
    ASSERT_NE(e, std::find(b, e, Move("c3e2")));
    ASSERT_NE(e, std::find(b, e, Move("c3d1")));

    rules.makeMove("c3e4");

    ASSERT_EQ(Status::Playing, rules.generateValidMoves());
    ASSERT_EQ(6, rules.m_legal_moves.size());
    b = rules.m_legal_moves.begin();
    e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("f7h8")));
    ASSERT_NE(e, std::find(b, e, Move("f7h6")));
    ASSERT_NE(e, std::find(b, e, Move("f7g5")));
    ASSERT_NE(e, std::find(b, e, Move("f7e5")));
    ASSERT_NE(e, std::find(b, e, Move("f7d6")));
    ASSERT_NE(e, std::find(b, e, Move("f7d8")));

    rules.makeMove("f7d6");
    ASSERT_EQ(Status::Playing, rules.generateValidMoves());
    ASSERT_EQ(8, rules.m_legal_moves.size());
    b = rules.m_legal_moves.begin();
    e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("e4d6")));
  }

  TEST(BlockedKnightMoves)
  {
    // Empty chessboard, No King, White Knight A1 blocks by
    // pawns in B3 and C2
    Rules rules(Color::White, Chessboard::Empty, WithNoKings);
    rules.m_board[sqA1] = WhiteKnight;
    rules.m_board[sqB3] = WhitePawn;
    rules.m_board[sqC2] = WhitePawn;
    ASSERT_EQ(Status::Playing, rules.generateValidMoves());
    ASSERT_EQ(3, rules.m_legal_moves.size());
    std::vector<Move>::iterator b = rules.m_legal_moves.begin();
    std::vector<Move>::iterator e = rules.m_legal_moves.end();
    ASSERT_EQ(e, std::find(b, e, Move("a1b3")));
    ASSERT_EQ(e, std::find(b, e, Move("a1c2")));
  }
}

TESTSUITE(BishopMoves)
{
  TEST(BasicBishopMoves)
  {
  }

  TEST(BlockedBoshopMoves)
  {
  }
}

TESTSUITE(RookMoves)
{
  TEST(BasicRookMoves)
  {
  }

  TEST(BlockedRookMoves)
  {
  }
}

TESTSUITE(QueenMoves)
{
  TEST(BasicQueenMoves)
  {
  }

  TEST(BlockedQueenMoves)
  {
  }
}

TESTSUITE(KingMoves)
{
  TEST(BasicKingMoves)
  {
  }

  TEST(BlockedKingMoves)
  {
  }

  TEST(TestInCheck)
  {
  }

  TEST(BlockedByCheck)
  {
  }

  TEST(AllowedCastle)
  {
  }

  TEST(NotAllowedCastle)
  {
  }
}

TESTSUITE(ChessStatus)
{
  TEST(WhiteCheckMate)
  {
    chessboard board;
    Color side;
    int ep;

    std::string FEN = "6k1/5ppp/8/8/8/8/8/R5K1 w k -";
    ASSERT_EQ(true, load(FEN, board, side, ep));
    ASSERT_EQ(Color::White, side);

    Rules rules(side, board);
    ASSERT_EQ(Status::Playing, rules.generateValidMoves());
    ASSERT_EQ(true, rules.m_legal_moves.size() > 1);

    rules.makeMove("a1a8");
    ASSERT_EQ(Status::WhiteWon, rules.generateValidMoves());
    ASSERT_EQ(0, rules.m_legal_moves.size());
  }

  TEST(BlackCheckMate)
  {
    chessboard board;
    Color side;
    int ep;

    std::string FEN = "k2r4/4b3/8/8/8/8/PPP5/2K5 b - -";
    ASSERT_EQ(true, load(FEN, board, side, ep));
    ASSERT_EQ(Color::Black, side);

    Rules rules(side, board);
    ASSERT_EQ(Status::Playing, rules.generateValidMoves());
    ASSERT_EQ(true, rules.m_legal_moves.size() > 1);

    rules.makeMove("e7g5");
    ASSERT_EQ(Status::Playing, rules.generateValidMoves());
    ASSERT_EQ(1, rules.m_legal_moves.size());
    std::vector<Move>::iterator b = rules.m_legal_moves.begin();
    std::vector<Move>::iterator e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("c1b1")));

    rules.makeMove("c1b1");
    rules.makeMove("d8d1");
    ASSERT_EQ(Status::BlackWon, rules.generateValidMoves());
    ASSERT_EQ(0, rules.m_legal_moves.size());
  }

  TEST(WhiteStalemate)
  {
    chessboard board;
    Color side;
    int ep;

    std::string FEN = "2k5/2P5/3K4/8/8/8/8/8 w - -";
    ASSERT_EQ(true, load(FEN, board, side, ep));
    ASSERT_EQ(Color::White, side);

    Rules rules(side, board);
    ASSERT_EQ(Status::Playing, rules.generateValidMoves());
    ASSERT_EQ(true, rules.m_legal_moves.size() > 1);

    rules.makeMove("d6c6");
    ASSERT_EQ(Status::Stalemate, rules.generateValidMoves());
    ASSERT_EQ(0, rules.m_legal_moves.size());
  }

  TEST(BlackStalemate)
  {
    chessboard board;
    Color side;
    int ep;

    std::string FEN = "8/8/8/8/8/5k2/6p1/6K1 b - -";
    ASSERT_EQ(true, load(FEN, board, side, ep));
    ASSERT_EQ(Color::Black, side);

    Rules rules(side, board);
    ASSERT_EQ(Status::Playing, rules.generateValidMoves());
    ASSERT_EQ(true, rules.m_legal_moves.size() > 1);

    rules.makeMove("f3g3");
    ASSERT_EQ(Status::Stalemate, rules.generateValidMoves());
    ASSERT_EQ(0, rules.m_legal_moves.size());
  }
}
