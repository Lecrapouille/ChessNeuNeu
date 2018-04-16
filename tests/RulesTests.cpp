#include "Chess/Rules.hpp"
#include <crpcut.hpp>
#include <iostream>
#include <ostream>
#include <algorithm>

TESTSUITE(Constructor)
{
  TEST(ConstructorFail, EXPECT_EXCEPTION(std::string))
    {
      // Not enough '/':
      //                       error
      //                         v
      Rules rules("6k1/5ppp/8/8/8/8/R5K1 b - -");
    }

  TEST(LoadFromMovesCorrect)
  {
    Rules rules1("rnbqkb1r/pppp1ppp/5n2/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq -");
    Rules rules2;

    ASSERT_EQ(true, rules2.m_moved == "");

    // Revert last move from init board
    ASSERT_EQ(true, Chessboard::Init == rules2.m_board);
    ASSERT_EQ(Color::White, rules2.m_side);
    rules2.revertLastMove();
    ASSERT_EQ(true, Chessboard::Init == rules2.m_board);
    ASSERT_EQ(Color::White, rules2.m_side);
    ASSERT_EQ(true, rules2.m_moved == "");

    // Load moves
    ASSERT_EQ(true, rules2.load("e2e4 e7e5 g1f3 g8f6 f1c4", true));
    ASSERT_EQ(true, rules1.m_board == rules2.m_board);
    ASSERT_EQ(Color::Black, rules2.m_side);
    ASSERT_EQ(true, rules2.m_moved == "e2e4 e7e5 g1f3 g8f6 f1c4");

    // Revert last move
    std::cout << "hjhjhjhhjh" << std::endl;
    rules2.revertLastMove();
    ASSERT_EQ(Color::White, rules2.m_side);
    std::cout << "COUPS " << rules2.m_moved << std::endl;
    ASSERT_EQ(true, rules2.m_moved == "e2e4 e7e5 g1f3 g8f6");
    Rules rules3 = rules1;
    rules3.m_board[sqC4] = NoPiece;
    rules3.m_board[sqF1] = WhiteBishop;
    ASSERT_EQ(true, rules3.m_board == rules2.m_board);

    // Revert last move
    rules2.revertLastMove();
    ASSERT_EQ(Color::Black, rules2.m_side);
    ASSERT_EQ(true, rules2.m_moved == "e2e4 e7e5 g1f3");
    rules2.revertLastMove();
    ASSERT_EQ(Color::White, rules2.m_side);
    ASSERT_EQ(true, rules2.m_moved == "e2e4 e7e5");
    rules2.revertLastMove();
    ASSERT_EQ(Color::Black, rules2.m_side);
    ASSERT_EQ(true, rules2.m_moved == "e2e4");
    rules2.revertLastMove();
    ASSERT_EQ(Color::White, rules2.m_side);
    ASSERT_EQ(true, rules2.m_moved == "");

    // Load moves
    ASSERT_EQ(true, rules2.load("e2e4 e7e5 g1f3 g8f6 f1c4", true));
    ASSERT_EQ(true, rules1.m_board == rules2.m_board);
    ASSERT_EQ(Color::Black, rules2.m_side);
    ASSERT_EQ(true, rules2.m_moved == "e2e4 e7e5 g1f3 g8f6 f1c4");
  }

  // TODO castle + revert
  // TODO  ep + revert

  TEST(LoadFromMovesFailure)
  {
    Rules rules0("rnbqkb1r/pppp1ppp/5n2/4p3/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq -");
    Rules rules1("rnbqkb1r/pppp1ppp/5n2/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq -");
    Rules rules2;

    ASSERT_EQ(false, rules2.load("e2e4 e7e5 g1f3 g8f6 f1b4", true));
    ASSERT_NE(true, rules1.m_board == rules2.m_board);
    ASSERT_EQ(true, rules0.m_board == rules2.m_board);
    ASSERT_EQ(Color::White, rules2.m_side);
  }
}

TESTSUITE(PawnMoves)
{
  TEST(NoMove)
  {
    // Empty chessboard. No Kings.
    Rules rules(Chessboard::Empty, Color::White, WithNoKings);

    // Check cannot take en passant, no castle allowed.
    ASSERT_EQ(Square::OOB, rules.m_ep);
    ASSERT_EQ(Castle::NoCastle, rules.m_castle[Color::White]);
    ASSERT_EQ(Castle::NoCastle, rules.m_castle[Color::Black]);

    // Check no available moves.
    rules.generateValidMoves();
    ASSERT_EQ(Status::NoMoveAvailable, rules.status());
    ASSERT_EQ(0, rules.m_legal_moves.size());
  }

  TEST(PawnMoveWE2_BE7)
  {
    // Empty chessboard, No King, White Pawn E2
    chessboard board = Chessboard::Empty;
    board[sqE2] = WhitePawn;
    board[sqE7] = BlackPawn;

    // Check legal moves
    Rules rules(board, Color::White, WithNoKings);
    ASSERT_EQ(Status::Playing, rules.status());
    ASSERT_EQ(2, rules.m_legal_moves.size());
    std::vector<Move>::iterator b = rules.m_legal_moves.begin();
    std::vector<Move>::iterator e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("e2e3")));
    ASSERT_NE(e, std::find(b, e, Move("e2e4")));

    ASSERT_EQ(true, rules.isValidMove("e2e3"));
    ASSERT_EQ(true, rules.isValidMove("e2e4"));
    ASSERT_EQ(false, rules.isValidMove("e2e5"));
    ASSERT_EQ(false, rules.isValidMove("e2f3"));
    ASSERT_EQ(false, rules.isValidMove("e2d3"));

    rules.applyMove("e2e3");

    rules.generateValidMoves();
    ASSERT_EQ(Status::Playing, rules.status());
    ASSERT_EQ(2, rules.m_legal_moves.size());
    b = rules.m_legal_moves.begin();
    e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("e7e6")));
    ASSERT_NE(e, std::find(b, e, Move("e7e5")));
  }

  TEST(PawnMoveWD3_BD6)
  {
    // Empty chessboard, No King, White Pawn D3
    chessboard board = Chessboard::Empty;
    board[sqD3] = WhitePawn;
    board[sqD6] = BlackPawn;

    // Check legal moves
    Rules rules(board, Color::White, WithNoKings);
    ASSERT_EQ(Status::Playing, rules.status());
    ASSERT_EQ(1, rules.m_legal_moves.size());
    std::vector<Move>::iterator b = rules.m_legal_moves.begin();
    std::vector<Move>::iterator e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("d3d4")));

    ASSERT_EQ(true, rules.isValidMove("d3d4"));
    ASSERT_EQ(false, rules.isValidMove("d4d5"));
    ASSERT_EQ(false, rules.isValidMove("d4f5"));
    ASSERT_EQ(false, rules.isValidMove("d4b5"));

    rules.applyMove("d3d4");

    rules.generateValidMoves();
    ASSERT_EQ(Status::Playing, rules.status());
    ASSERT_EQ(1, rules.m_legal_moves.size());
    b = rules.m_legal_moves.begin();
    e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("d6d5")));
  }

  TEST(PawnMoveWA4_BA5)
  {
    // Empty chessboard, No King, White Pawn A4
    chessboard board = Chessboard::Empty;
    board[sqA4] = WhitePawn;
    board[sqA5] = BlackPawn;

    // Check no longer moves.
    Rules rules(board, Color::White, WithNoKings);
    ASSERT_EQ(Status::NoMoveAvailable, rules.status());
    ASSERT_EQ(0, rules.m_legal_moves.size());
  }

  TEST(PawnMoveWC4_BB5)
  {
    // Empty chessboard, No King, White Pawn C4
    chessboard board = Chessboard::Empty;
    board[sqA4] = WhitePawn;
    board[sqC4] = WhitePawn;
    board[sqB5] = BlackPawn;

    // Check legal moves
    Rules rules(board, Color::White, WithNoKings);
    ASSERT_EQ(Status::Playing, rules.status());
    ASSERT_EQ(4, rules.m_legal_moves.size());
    std::vector<Move>::iterator b = rules.m_legal_moves.begin();
    std::vector<Move>::iterator e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("c4c5")));
    ASSERT_NE(e, std::find(b, e, Move("c4b5")));
    ASSERT_NE(e, std::find(b, e, Move("a4a5")));
    ASSERT_NE(e, std::find(b, e, Move("a4b5")));

    rules.m_side = Color::Black;
    rules.generateValidMoves();
    ASSERT_EQ(Status::Playing, rules.status());
    ASSERT_EQ(3, rules.m_legal_moves.size());
    b = rules.m_legal_moves.begin();
    e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("b5a4")));
    ASSERT_NE(e, std::find(b, e, Move("b5c4")));
    ASSERT_NE(e, std::find(b, e, Move("b5b4")));
  }

  TEST(PawnPromotionMove)
  {
    // Empty chessboard, No King, White Pawn G7
    chessboard board = Chessboard::Empty;
    board[sqG7] = WhitePawn;
    board[sqB2] = BlackPawn;

    // Check legal moves
    Rules rules(board, Color::White, WithNoKings);
    ASSERT_EQ(Status::Playing, rules.status());
    ASSERT_EQ(4, rules.m_legal_moves.size());
    std::vector<Move>::iterator b = rules.m_legal_moves.begin();
    std::vector<Move>::iterator e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("g7g8r")));
    ASSERT_NE(e, std::find(b, e, Move("g7g8n")));
    ASSERT_NE(e, std::find(b, e, Move("g7g8b")));
    ASSERT_NE(e, std::find(b, e, Move("g7g8q")));

    rules.applyMove("g7g8b");
    ASSERT_EQ(true, WhiteBishop == rules.m_board[sqG8]);

    rules.generateValidMoves();
    ASSERT_EQ(Status::Playing, rules.status());
    ASSERT_EQ(4, rules.m_legal_moves.size());
    b = rules.m_legal_moves.begin();
    e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("b2b1r")));
    ASSERT_NE(e, std::find(b, e, Move("b2b1n")));
    ASSERT_NE(e, std::find(b, e, Move("b2b1b")));
    ASSERT_NE(e, std::find(b, e, Move("b2b1q")));

    rules.applyMove("b2b1r");
    ASSERT_EQ(BlackRook, rules.m_board[sqB1]);
  }

  TEST(WhiteEnPassantAllowed)
  {
    Rules rules;
    ASSERT_EQ(true, rules.load("8/8/8/3pP3/8/8/8/8 w - d6"));

    ASSERT_EQ(Color::White, rules.m_side);
    ASSERT_EQ(sqD6, rules.m_ep);
    ASSERT_EQ(WithNoKings, rules.hasNoKing);
    ASSERT_EQ(2, rules.m_legal_moves.size());

    // Check movement
    std::vector<Move>::iterator b = rules.m_legal_moves.begin();
    std::vector<Move>::iterator e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("e5e6")));
    ASSERT_NE(e, std::find(b, e, Move("e5d6")));

    // Check pieces after prise en passant
    rules.applyMove("e5d6");
    ASSERT_EQ(WhitePawn, rules.m_board[sqD6]);
    ASSERT_EQ(NoPiece, rules.m_board[sqD5]);
    ASSERT_EQ(NoPiece, rules.m_board[sqE5]);

    ASSERT_EQ(Square::OOB, rules.m_ep);
  }

  TEST(BlackEnPassantAllowed)
  {
    Rules rules;
    ASSERT_EQ(true, rules.load("8/8/8/8/3pP3/8/8/8 b - e3"));

    ASSERT_EQ(Color::Black, rules.m_side);
    ASSERT_EQ(sqE3, rules.m_ep);
    ASSERT_EQ(WithNoKings, rules.hasNoKing);
    ASSERT_EQ(2, rules.m_legal_moves.size());

    // Check movement
    std::vector<Move>::iterator b = rules.m_legal_moves.begin();
    std::vector<Move>::iterator e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("d4d3")));
    ASSERT_NE(e, std::find(b, e, Move("d4e3")));

    // Check pieces after prise en passant
    rules.applyMove("d4e3");
    ASSERT_EQ(BlackPawn, rules.m_board[sqE3]);
    ASSERT_EQ(NoPiece, rules.m_board[sqD4]);
    ASSERT_EQ(NoPiece, rules.m_board[sqE4]);

    ASSERT_EQ(Square::OOB, rules.m_ep);
  }

  TEST(EnPassantNotAllowed)
  {
    Rules rules;
    ASSERT_EQ(true, rules.load("8/8/8/8/3pP3/8/8/8 b - -"));

    ASSERT_EQ(Color::Black, rules.m_side);
    ASSERT_EQ(Square::OOB, rules.m_ep);
    ASSERT_EQ(WithNoKings, rules.hasNoKing);
    ASSERT_EQ(1, rules.m_legal_moves.size());

    std::vector<Move>::iterator b = rules.m_legal_moves.begin();
    std::vector<Move>::iterator e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("d4d3")));
  }

  TEST(GenerateWhiteEnPassant)
  {
    Rules rules;
    ASSERT_EQ(true, rules.load("8/3p4/8/4P3/8/8/8/8 b - -"));

    ASSERT_EQ(Color::Black, rules.m_side);
    ASSERT_EQ(Square::OOB, rules.m_ep);
    ASSERT_EQ(WithNoKings, rules.hasNoKing);

    rules.applyMove("d7d5");
    ASSERT_EQ(sqD6, rules.m_ep);
    ASSERT_EQ(2, rules.m_legal_moves.size());

    std::vector<Move>::iterator b = rules.m_legal_moves.begin();
    std::vector<Move>::iterator e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("e5e6")));
    ASSERT_NE(e, std::find(b, e, Move("e5d6")));
  }

  TEST(GenerateBlackEnPassant)
  {
    Rules rules;
    ASSERT_EQ(true, rules.load("8/8/8/8/3p4/8/4P3/8 w - -"));

    ASSERT_EQ(Color::White, rules.m_side);
    ASSERT_EQ(Square::OOB, rules.m_ep);
    ASSERT_EQ(WithNoKings, rules.hasNoKing);

    rules.applyMove("e2e4");
    ASSERT_EQ(sqE3, rules.m_ep);
    ASSERT_EQ(2, rules.m_legal_moves.size());

    std::vector<Move>::iterator b = rules.m_legal_moves.begin();
    std::vector<Move>::iterator e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("d4d3")));
    ASSERT_NE(e, std::find(b, e, Move("d4e3")));
  }
}

TESTSUITE(KnightMoves)
{
  TEST(BasicKnightMoves)
  {
    // Empty chessboard, No King, White Knight B1, Black Knight H8
    Rules rules(Chessboard::Empty, Color::White, WithNoKings);
    rules.m_board[sqB1] = WhiteKnight;
    rules.m_board[sqH8] = BlackKnight;

    // Check legal moves
    rules.generateValidMoves();
    ASSERT_EQ(Status::Playing, rules.status());
    ASSERT_EQ(3, rules.m_legal_moves.size());
    std::vector<Move>::iterator b = rules.m_legal_moves.begin();
    std::vector<Move>::iterator e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("b1a3")));
    ASSERT_NE(e, std::find(b, e, Move("b1c3")));
    ASSERT_NE(e, std::find(b, e, Move("b1d2")));

    rules.applyMove("b1c3");

    rules.generateValidMoves();
    ASSERT_EQ(Status::Playing, rules.status());
    ASSERT_EQ(2, rules.m_legal_moves.size());
    b = rules.m_legal_moves.begin();
    e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("h8f7")));
    ASSERT_NE(e, std::find(b, e, Move("h8g6")));

    rules.applyMove("h8f7");

    rules.generateValidMoves();
    ASSERT_EQ(Status::Playing, rules.status());
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

    rules.applyMove("c3e4");

    rules.generateValidMoves();
    ASSERT_EQ(Status::Playing, rules.status());
    ASSERT_EQ(6, rules.m_legal_moves.size());
    b = rules.m_legal_moves.begin();
    e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("f7h8")));
    ASSERT_NE(e, std::find(b, e, Move("f7h6")));
    ASSERT_NE(e, std::find(b, e, Move("f7g5")));
    ASSERT_NE(e, std::find(b, e, Move("f7e5")));
    ASSERT_NE(e, std::find(b, e, Move("f7d6")));
    ASSERT_NE(e, std::find(b, e, Move("f7d8")));

    rules.applyMove("f7d6");
    rules.generateValidMoves();
    ASSERT_EQ(Status::Playing, rules.status());
    ASSERT_EQ(8, rules.m_legal_moves.size());//kkkkkkkkk
    b = rules.m_legal_moves.begin();
    e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("e4d6")));
  }

  TEST(BlockedKnightMoves)
  {
    // Empty chessboard, No King, White Knight A1 blocks by
    // pawns in B3 and C2
    Rules rules(Chessboard::Empty, Color::White, WithNoKings);
    rules.m_board[sqA1] = WhiteKnight;
    rules.m_board[sqB3] = WhitePawn;
    rules.m_board[sqC2] = WhitePawn;
    rules.generateValidMoves();
    ASSERT_EQ(Status::Playing, rules.status());
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
    Rules rules;
    ASSERT_EQ(true, rules.load("8/8/3b4/8/3BB1b1/8/8/8 w - -"));

    ASSERT_EQ(Color::White, rules.m_side);
    ASSERT_EQ(WithNoKings, rules.hasNoKing);
    ASSERT_EQ(Status::Playing, rules.status());
    ASSERT_EQ(26, rules.m_legal_moves.size());
    std::vector<Move>::iterator b = rules.m_legal_moves.begin();
    std::vector<Move>::iterator e = rules.m_legal_moves.end();

    ASSERT_NE(e, std::find(b, e, Move("d4e5")));
    ASSERT_NE(e, std::find(b, e, Move("d4f6")));
    ASSERT_NE(e, std::find(b, e, Move("d4g7")));
    ASSERT_NE(e, std::find(b, e, Move("d4h8")));
    ASSERT_NE(e, std::find(b, e, Move("d4c5")));
    ASSERT_NE(e, std::find(b, e, Move("d4b6")));
    ASSERT_NE(e, std::find(b, e, Move("d4a7")));
    ASSERT_NE(e, std::find(b, e, Move("d4e3")));
    ASSERT_NE(e, std::find(b, e, Move("d4f2")));
    ASSERT_NE(e, std::find(b, e, Move("d4g1")));
    ASSERT_NE(e, std::find(b, e, Move("d4c3")));
    ASSERT_NE(e, std::find(b, e, Move("d4b2")));
    ASSERT_NE(e, std::find(b, e, Move("d4a1")));

    ASSERT_NE(e, std::find(b, e, Move("e4f5")));
    ASSERT_NE(e, std::find(b, e, Move("e4g6")));
    ASSERT_NE(e, std::find(b, e, Move("e4h7")));
    ASSERT_NE(e, std::find(b, e, Move("e4d5")));
    ASSERT_NE(e, std::find(b, e, Move("e4c6")));
    ASSERT_NE(e, std::find(b, e, Move("e4b7")));
    ASSERT_NE(e, std::find(b, e, Move("e4a8")));
    ASSERT_NE(e, std::find(b, e, Move("e4f3")));
    ASSERT_NE(e, std::find(b, e, Move("e4g2")));
    ASSERT_NE(e, std::find(b, e, Move("e4h1")));
    ASSERT_NE(e, std::find(b, e, Move("e4d3")));
    ASSERT_NE(e, std::find(b, e, Move("e4c2")));
    ASSERT_NE(e, std::find(b, e, Move("e4b1")));

    rules.applyMove("e4d5");
    ASSERT_EQ(Status::Playing, rules.status());
    ASSERT_EQ(20, rules.m_legal_moves.size());
    b = rules.m_legal_moves.begin();
    e = rules.m_legal_moves.end();

    ASSERT_NE(e, std::find(b, e, Move("d6e7")));
    ASSERT_NE(e, std::find(b, e, Move("d6f8")));
    ASSERT_NE(e, std::find(b, e, Move("d6c7")));
    ASSERT_NE(e, std::find(b, e, Move("d6b8")));
    ASSERT_NE(e, std::find(b, e, Move("d6e5")));
    ASSERT_NE(e, std::find(b, e, Move("d6f4")));
    ASSERT_NE(e, std::find(b, e, Move("d6g3")));
    ASSERT_NE(e, std::find(b, e, Move("d6h2")));
    ASSERT_NE(e, std::find(b, e, Move("d6c5")));
    ASSERT_NE(e, std::find(b, e, Move("d6b4")));
    ASSERT_NE(e, std::find(b, e, Move("d6a3")));

    ASSERT_NE(e, std::find(b, e, Move("g4h5")));
    ASSERT_NE(e, std::find(b, e, Move("g4f5")));
    ASSERT_NE(e, std::find(b, e, Move("g4e6")));
    ASSERT_NE(e, std::find(b, e, Move("g4d7")));
    ASSERT_NE(e, std::find(b, e, Move("g4c8")));
    ASSERT_NE(e, std::find(b, e, Move("g4h3")));
    ASSERT_NE(e, std::find(b, e, Move("g4f3")));
    ASSERT_NE(e, std::find(b, e, Move("g4e2")));
    ASSERT_NE(e, std::find(b, e, Move("g4d1")));
  }

  TEST(BlockedBoshopMoves)
  {
    Rules rules;
    ASSERT_EQ(true, rules.load("8/3B2b1/3b4/8/3BB1b1/8/2B1b3/8 w - -"));

    ASSERT_EQ(Color::White, rules.m_side);
    ASSERT_EQ(WithNoKings, rules.hasNoKing);
    ASSERT_EQ(Status::Playing, rules.status());
    ASSERT_EQ(36, rules.m_legal_moves.size());
    std::vector<Move>::iterator b = rules.m_legal_moves.begin();
    std::vector<Move>::iterator e = rules.m_legal_moves.end();

    ASSERT_NE(e, std::find(b, e, Move("d7e8")));
    ASSERT_NE(e, std::find(b, e, Move("d7c8")));
    ASSERT_NE(e, std::find(b, e, Move("d7e6")));
    ASSERT_NE(e, std::find(b, e, Move("d7f5")));
    ASSERT_NE(e, std::find(b, e, Move("d7g4")));
    ASSERT_NE(e, std::find(b, e, Move("d7c6")));
    ASSERT_NE(e, std::find(b, e, Move("d7b5")));
    ASSERT_NE(e, std::find(b, e, Move("d7a4")));
    ASSERT_NE(e, std::find(b, e, Move("d4e5")));
    ASSERT_NE(e, std::find(b, e, Move("d4f6")));
    ASSERT_NE(e, std::find(b, e, Move("d4g7")));
    ASSERT_NE(e, std::find(b, e, Move("d4c5")));
    ASSERT_NE(e, std::find(b, e, Move("d4b6")));
    ASSERT_NE(e, std::find(b, e, Move("d4a7")));
    ASSERT_NE(e, std::find(b, e, Move("d4e3")));
    ASSERT_NE(e, std::find(b, e, Move("d4f2")));
    ASSERT_NE(e, std::find(b, e, Move("d4g1")));
    ASSERT_NE(e, std::find(b, e, Move("d4c3")));
    ASSERT_NE(e, std::find(b, e, Move("d4b2")));
    ASSERT_NE(e, std::find(b, e, Move("d4a1")));
    ASSERT_NE(e, std::find(b, e, Move("e4f5")));
    ASSERT_NE(e, std::find(b, e, Move("e4g6")));
    ASSERT_NE(e, std::find(b, e, Move("e4h7")));
    ASSERT_NE(e, std::find(b, e, Move("e4d5")));
    ASSERT_NE(e, std::find(b, e, Move("e4c6")));
    ASSERT_NE(e, std::find(b, e, Move("e4b7")));
    ASSERT_NE(e, std::find(b, e, Move("e4a8")));
    ASSERT_NE(e, std::find(b, e, Move("e4f3")));
    ASSERT_NE(e, std::find(b, e, Move("e4g2")));
    ASSERT_NE(e, std::find(b, e, Move("e4h1")));
    ASSERT_NE(e, std::find(b, e, Move("e4d3")));
    ASSERT_NE(e, std::find(b, e, Move("c2d3")));
    ASSERT_NE(e, std::find(b, e, Move("c2b3")));
    ASSERT_NE(e, std::find(b, e, Move("c2a4")));
    ASSERT_NE(e, std::find(b, e, Move("c2d1")));
    ASSERT_NE(e, std::find(b, e, Move("c2b1")));

    rules.applyMove("e4f5");
    ASSERT_EQ(Status::Playing, rules.status());
    ASSERT_EQ(28, rules.m_legal_moves.size());
    b = rules.m_legal_moves.begin();
    e = rules.m_legal_moves.end();

    ASSERT_NE(e, std::find(b, e, Move("g7h8")));
    ASSERT_NE(e, std::find(b, e, Move("g7f8")));
    ASSERT_NE(e, std::find(b, e, Move("g7h6")));
    ASSERT_NE(e, std::find(b, e, Move("g7f6")));
    ASSERT_NE(e, std::find(b, e, Move("g7e5")));
    ASSERT_NE(e, std::find(b, e, Move("g7d4")));
    ASSERT_NE(e, std::find(b, e, Move("d6e7")));
    ASSERT_NE(e, std::find(b, e, Move("d6f8")));
    ASSERT_NE(e, std::find(b, e, Move("d6c7")));
    ASSERT_NE(e, std::find(b, e, Move("d6b8")));
    ASSERT_NE(e, std::find(b, e, Move("d6e5")));
    ASSERT_NE(e, std::find(b, e, Move("d6f4")));
    ASSERT_NE(e, std::find(b, e, Move("d6g3")));
    ASSERT_NE(e, std::find(b, e, Move("d6h2")));
    ASSERT_NE(e, std::find(b, e, Move("d6c5")));
    ASSERT_NE(e, std::find(b, e, Move("d6b4")));
    ASSERT_NE(e, std::find(b, e, Move("d6a3")));
    ASSERT_NE(e, std::find(b, e, Move("g4h5")));
    ASSERT_NE(e, std::find(b, e, Move("g4f5")));
    ASSERT_NE(e, std::find(b, e, Move("g4h3")));
    ASSERT_NE(e, std::find(b, e, Move("g4f3")));
    ASSERT_NE(e, std::find(b, e, Move("e2f3")));
    ASSERT_NE(e, std::find(b, e, Move("e2d3")));
    ASSERT_NE(e, std::find(b, e, Move("e2c4")));
    ASSERT_NE(e, std::find(b, e, Move("e2b5")));
    ASSERT_NE(e, std::find(b, e, Move("e2a6")));
    ASSERT_NE(e, std::find(b, e, Move("e2f1")));
    ASSERT_NE(e, std::find(b, e, Move("e2d1")));
  }
}

TESTSUITE(RookMoves)
{
  TEST(BasicRookMoves)
  {
    Rules rules;
    ASSERT_EQ(true, rules.load("8/6r1/8/3R4/4R3/2r5/8/8 w - -"));

    ASSERT_EQ(Color::White, rules.m_side);
    ASSERT_EQ(WithNoKings, rules.hasNoKing);
    ASSERT_EQ(Status::Playing, rules.status());
    ASSERT_EQ(28, rules.m_legal_moves.size());
    std::vector<Move>::iterator b = rules.m_legal_moves.begin();
    std::vector<Move>::iterator e = rules.m_legal_moves.end();

    ASSERT_NE(e, std::find(b, e, Move("d5d6")));
    ASSERT_NE(e, std::find(b, e, Move("d5d7")));
    ASSERT_NE(e, std::find(b, e, Move("d5d8")));
    ASSERT_NE(e, std::find(b, e, Move("d5e5")));
    ASSERT_NE(e, std::find(b, e, Move("d5f5")));
    ASSERT_NE(e, std::find(b, e, Move("d5g5")));
    ASSERT_NE(e, std::find(b, e, Move("d5h5")));
    ASSERT_NE(e, std::find(b, e, Move("d5d4")));
    ASSERT_NE(e, std::find(b, e, Move("d5d3")));
    ASSERT_NE(e, std::find(b, e, Move("d5d2")));
    ASSERT_NE(e, std::find(b, e, Move("d5d1")));
    ASSERT_NE(e, std::find(b, e, Move("d5c5")));
    ASSERT_NE(e, std::find(b, e, Move("d5b5")));
    ASSERT_NE(e, std::find(b, e, Move("d5a5")));
    ASSERT_NE(e, std::find(b, e, Move("e4e5")));
    ASSERT_NE(e, std::find(b, e, Move("e4e6")));
    ASSERT_NE(e, std::find(b, e, Move("e4e7")));
    ASSERT_NE(e, std::find(b, e, Move("e4e8")));
    ASSERT_NE(e, std::find(b, e, Move("e4f4")));
    ASSERT_NE(e, std::find(b, e, Move("e4g4")));
    ASSERT_NE(e, std::find(b, e, Move("e4h4")));
    ASSERT_NE(e, std::find(b, e, Move("e4e3")));
    ASSERT_NE(e, std::find(b, e, Move("e4e2")));
    ASSERT_NE(e, std::find(b, e, Move("e4e1")));
    ASSERT_NE(e, std::find(b, e, Move("e4d4")));
    ASSERT_NE(e, std::find(b, e, Move("e4c4")));
    ASSERT_NE(e, std::find(b, e, Move("e4b4")));
    ASSERT_NE(e, std::find(b, e, Move("e4a4")));

    rules.applyMove("e4e5");
    ASSERT_EQ(Status::Playing, rules.status());
    ASSERT_EQ(28, rules.m_legal_moves.size());
    b = rules.m_legal_moves.begin();
    e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("g7g8")));
    ASSERT_NE(e, std::find(b, e, Move("g7h7")));
    ASSERT_NE(e, std::find(b, e, Move("g7g6")));
    ASSERT_NE(e, std::find(b, e, Move("g7g5")));
    ASSERT_NE(e, std::find(b, e, Move("g7g4")));
    ASSERT_NE(e, std::find(b, e, Move("g7g3")));
    ASSERT_NE(e, std::find(b, e, Move("g7g2")));
    ASSERT_NE(e, std::find(b, e, Move("g7g1")));
    ASSERT_NE(e, std::find(b, e, Move("g7f7")));
    ASSERT_NE(e, std::find(b, e, Move("g7e7")));
    ASSERT_NE(e, std::find(b, e, Move("g7d7")));
    ASSERT_NE(e, std::find(b, e, Move("g7c7")));
    ASSERT_NE(e, std::find(b, e, Move("g7b7")));
    ASSERT_NE(e, std::find(b, e, Move("g7a7")));
    ASSERT_NE(e, std::find(b, e, Move("c3c4")));
    ASSERT_NE(e, std::find(b, e, Move("c3c5")));
    ASSERT_NE(e, std::find(b, e, Move("c3c6")));
    ASSERT_NE(e, std::find(b, e, Move("c3c7")));
    ASSERT_NE(e, std::find(b, e, Move("c3c8")));
    ASSERT_NE(e, std::find(b, e, Move("c3d3")));
    ASSERT_NE(e, std::find(b, e, Move("c3e3")));
    ASSERT_NE(e, std::find(b, e, Move("c3f3")));
    ASSERT_NE(e, std::find(b, e, Move("c3g3")));
    ASSERT_NE(e, std::find(b, e, Move("c3h3")));
    ASSERT_NE(e, std::find(b, e, Move("c3c2")));
    ASSERT_NE(e, std::find(b, e, Move("c3c1")));
    ASSERT_NE(e, std::find(b, e, Move("c3b3")));
    ASSERT_NE(e, std::find(b, e, Move("c3a3")));
  }

  TEST(BlockedRookMoves)
  {
    Rules rules;
    ASSERT_EQ(true, rules.load("8/1r1R4/4r3/3R4/4R1r1/1r3R2/8/8 w - -"));

    ASSERT_EQ(Color::White, rules.m_side);
    ASSERT_EQ(WithNoKings, rules.hasNoKing);
    ASSERT_EQ(Status::Playing, rules.status());
    ASSERT_EQ(44, rules.m_legal_moves.size());
    std::vector<Move>::iterator b = rules.m_legal_moves.begin();
    std::vector<Move>::iterator e = rules.m_legal_moves.end();

    ASSERT_NE(e, std::find(b, e, Move("d7d8")));
    ASSERT_NE(e, std::find(b, e, Move("d7e7")));
    ASSERT_NE(e, std::find(b, e, Move("d7f7")));
    ASSERT_NE(e, std::find(b, e, Move("d7g7")));
    ASSERT_NE(e, std::find(b, e, Move("d7h7")));
    ASSERT_NE(e, std::find(b, e, Move("d7d6")));
    ASSERT_NE(e, std::find(b, e, Move("d7c7")));
    ASSERT_NE(e, std::find(b, e, Move("d7b7")));
    ASSERT_NE(e, std::find(b, e, Move("d5d6")));
    ASSERT_NE(e, std::find(b, e, Move("d5e5")));
    ASSERT_NE(e, std::find(b, e, Move("d5f5")));
    ASSERT_NE(e, std::find(b, e, Move("d5g5")));
    ASSERT_NE(e, std::find(b, e, Move("d5h5")));
    ASSERT_NE(e, std::find(b, e, Move("d5d4")));
    ASSERT_NE(e, std::find(b, e, Move("d5d3")));
    ASSERT_NE(e, std::find(b, e, Move("d5d2")));
    ASSERT_NE(e, std::find(b, e, Move("d5d1")));
    ASSERT_NE(e, std::find(b, e, Move("d5c5")));
    ASSERT_NE(e, std::find(b, e, Move("d5b5")));
    ASSERT_NE(e, std::find(b, e, Move("d5a5")));
    ASSERT_NE(e, std::find(b, e, Move("e4e5")));
    ASSERT_NE(e, std::find(b, e, Move("e4e6")));
    ASSERT_NE(e, std::find(b, e, Move("e4f4")));
    ASSERT_NE(e, std::find(b, e, Move("e4g4")));
    ASSERT_NE(e, std::find(b, e, Move("e4e3")));
    ASSERT_NE(e, std::find(b, e, Move("e4e2")));
    ASSERT_NE(e, std::find(b, e, Move("e4e1")));
    ASSERT_NE(e, std::find(b, e, Move("e4d4")));
    ASSERT_NE(e, std::find(b, e, Move("e4c4")));
    ASSERT_NE(e, std::find(b, e, Move("e4b4")));
    ASSERT_NE(e, std::find(b, e, Move("e4a4")));
    ASSERT_NE(e, std::find(b, e, Move("f3f4")));
    ASSERT_NE(e, std::find(b, e, Move("f3f5")));
    ASSERT_NE(e, std::find(b, e, Move("f3f6")));
    ASSERT_NE(e, std::find(b, e, Move("f3f7")));
    ASSERT_NE(e, std::find(b, e, Move("f3f8")));
    ASSERT_NE(e, std::find(b, e, Move("f3g3")));
    ASSERT_NE(e, std::find(b, e, Move("f3h3")));
    ASSERT_NE(e, std::find(b, e, Move("f3f2")));
    ASSERT_NE(e, std::find(b, e, Move("f3f1")));
    ASSERT_NE(e, std::find(b, e, Move("f3e3")));
    ASSERT_NE(e, std::find(b, e, Move("f3d3")));
    ASSERT_NE(e, std::find(b, e, Move("f3c3")));
    ASSERT_NE(e, std::find(b, e, Move("f3b3")));

    rules.applyMove("f3b3");
    ASSERT_EQ(Status::Playing, rules.status());
    ASSERT_EQ(29, rules.m_legal_moves.size());
    b = rules.m_legal_moves.begin();
    e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("b7b8")));
    ASSERT_NE(e, std::find(b, e, Move("b7c7")));
    ASSERT_NE(e, std::find(b, e, Move("b7d7")));
    ASSERT_NE(e, std::find(b, e, Move("b7b6")));
    ASSERT_NE(e, std::find(b, e, Move("b7b5")));
    ASSERT_NE(e, std::find(b, e, Move("b7b4")));
    ASSERT_NE(e, std::find(b, e, Move("b7b3")));
    ASSERT_NE(e, std::find(b, e, Move("b7a7")));
    ASSERT_NE(e, std::find(b, e, Move("e6e7")));
    ASSERT_NE(e, std::find(b, e, Move("e6e8")));
    ASSERT_NE(e, std::find(b, e, Move("e6f6")));
    ASSERT_NE(e, std::find(b, e, Move("e6g6")));
    ASSERT_NE(e, std::find(b, e, Move("e6h6")));
    ASSERT_NE(e, std::find(b, e, Move("e6e5")));
    ASSERT_NE(e, std::find(b, e, Move("e6e4")));
    ASSERT_NE(e, std::find(b, e, Move("e6d6")));
    ASSERT_NE(e, std::find(b, e, Move("e6c6")));
    ASSERT_NE(e, std::find(b, e, Move("e6b6")));
    ASSERT_NE(e, std::find(b, e, Move("e6a6")));
    ASSERT_NE(e, std::find(b, e, Move("g4g5")));
    ASSERT_NE(e, std::find(b, e, Move("g4g6")));
    ASSERT_NE(e, std::find(b, e, Move("g4g7")));
    ASSERT_NE(e, std::find(b, e, Move("g4g8")));
    ASSERT_NE(e, std::find(b, e, Move("g4h4")));
    ASSERT_NE(e, std::find(b, e, Move("g4g3")));
    ASSERT_NE(e, std::find(b, e, Move("g4g2")));
    ASSERT_NE(e, std::find(b, e, Move("g4g1")));
    ASSERT_NE(e, std::find(b, e, Move("g4f4")));
    ASSERT_NE(e, std::find(b, e, Move("g4e4")));
  }
}

TESTSUITE(QueenMoves)
{
  TEST(BasicQueenMoves)
  {
    Rules rules;
    ASSERT_EQ(true, rules.load("8/4q3/8/1Q6/3Q4/8/2q5/8 w - -"));

    ASSERT_EQ(Color::White, rules.m_side);
    ASSERT_EQ(WithNoKings, rules.hasNoKing);
    ASSERT_EQ(Status::Playing, rules.status());
    ASSERT_EQ(50, rules.m_legal_moves.size());
    std::vector<Move>::iterator b = rules.m_legal_moves.begin();
    std::vector<Move>::iterator e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("b5b6")));
    ASSERT_NE(e, std::find(b, e, Move("b5b7")));
    ASSERT_NE(e, std::find(b, e, Move("b5b8")));
    ASSERT_NE(e, std::find(b, e, Move("b5b4")));
    ASSERT_NE(e, std::find(b, e, Move("b5b3")));
    ASSERT_NE(e, std::find(b, e, Move("b5b2")));
    ASSERT_NE(e, std::find(b, e, Move("b5b1")));
    ASSERT_NE(e, std::find(b, e, Move("b5c5")));
    ASSERT_NE(e, std::find(b, e, Move("b5d5")));
    ASSERT_NE(e, std::find(b, e, Move("b5e5")));
    ASSERT_NE(e, std::find(b, e, Move("b5f5")));
    ASSERT_NE(e, std::find(b, e, Move("b5g5")));
    ASSERT_NE(e, std::find(b, e, Move("b5h5")));
    ASSERT_NE(e, std::find(b, e, Move("b5a5")));
    ASSERT_NE(e, std::find(b, e, Move("b5a6")));
    ASSERT_NE(e, std::find(b, e, Move("b5c6")));
    ASSERT_NE(e, std::find(b, e, Move("b5d7")));
    ASSERT_NE(e, std::find(b, e, Move("b5e8")));
    ASSERT_NE(e, std::find(b, e, Move("b5c4")));
    ASSERT_NE(e, std::find(b, e, Move("b5d3")));
    ASSERT_NE(e, std::find(b, e, Move("b5e2")));
    ASSERT_NE(e, std::find(b, e, Move("b5f1")));
    ASSERT_NE(e, std::find(b, e, Move("b5a4")));
    ASSERT_NE(e, std::find(b, e, Move("d4d5")));
    ASSERT_NE(e, std::find(b, e, Move("d4d6")));
    ASSERT_NE(e, std::find(b, e, Move("d4d7")));
    ASSERT_NE(e, std::find(b, e, Move("d4d8")));
    ASSERT_NE(e, std::find(b, e, Move("d4d3")));
    ASSERT_NE(e, std::find(b, e, Move("d4d2")));
    ASSERT_NE(e, std::find(b, e, Move("d4d1")));
    ASSERT_NE(e, std::find(b, e, Move("d4e4")));
    ASSERT_NE(e, std::find(b, e, Move("d4f4")));
    ASSERT_NE(e, std::find(b, e, Move("d4g4")));
    ASSERT_NE(e, std::find(b, e, Move("d4h4")));
    ASSERT_NE(e, std::find(b, e, Move("d4c4")));
    ASSERT_NE(e, std::find(b, e, Move("d4b4")));
    ASSERT_NE(e, std::find(b, e, Move("d4a4")));
    ASSERT_NE(e, std::find(b, e, Move("d4c5")));
    ASSERT_NE(e, std::find(b, e, Move("d4b6")));
    ASSERT_NE(e, std::find(b, e, Move("d4a7")));
    ASSERT_NE(e, std::find(b, e, Move("d4e5")));
    ASSERT_NE(e, std::find(b, e, Move("d4f6")));
    ASSERT_NE(e, std::find(b, e, Move("d4g7")));
    ASSERT_NE(e, std::find(b, e, Move("d4h8")));
    ASSERT_NE(e, std::find(b, e, Move("d4e3")));
    ASSERT_NE(e, std::find(b, e, Move("d4f2")));
    ASSERT_NE(e, std::find(b, e, Move("d4g1")));
    ASSERT_NE(e, std::find(b, e, Move("d4c3")));
    ASSERT_NE(e, std::find(b, e, Move("d4b2")));
    ASSERT_NE(e, std::find(b, e, Move("d4a1")));
  }

  TEST(BlockedQueenMoves)
  {
    Rules rules;
    ASSERT_EQ(true, rules.load("8/4q3/8/2Q5/4Q3/8/2q5/8 w - -"));

    ASSERT_EQ(Color::White, rules.m_side);
    ASSERT_EQ(WithNoKings, rules.hasNoKing);
    ASSERT_EQ(Status::Playing, rules.status());
    ASSERT_EQ(48, rules.m_legal_moves.size());
    std::vector<Move>::iterator b = rules.m_legal_moves.begin();
    std::vector<Move>::iterator e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("c5c6")));
    ASSERT_NE(e, std::find(b, e, Move("c5c7")));
    ASSERT_NE(e, std::find(b, e, Move("c5c8")));
    ASSERT_NE(e, std::find(b, e, Move("c5c4")));
    ASSERT_NE(e, std::find(b, e, Move("c5c3")));
    ASSERT_NE(e, std::find(b, e, Move("c5c2")));
    ASSERT_NE(e, std::find(b, e, Move("c5d5")));
    ASSERT_NE(e, std::find(b, e, Move("c5e5")));
    ASSERT_NE(e, std::find(b, e, Move("c5f5")));
    ASSERT_NE(e, std::find(b, e, Move("c5g5")));
    ASSERT_NE(e, std::find(b, e, Move("c5h5")));
    ASSERT_NE(e, std::find(b, e, Move("c5b5")));
    ASSERT_NE(e, std::find(b, e, Move("c5a5")));
    ASSERT_NE(e, std::find(b, e, Move("c5b6")));
    ASSERT_NE(e, std::find(b, e, Move("c5a7")));
    ASSERT_NE(e, std::find(b, e, Move("c5d6")));
    ASSERT_NE(e, std::find(b, e, Move("c5e7")));
    ASSERT_NE(e, std::find(b, e, Move("c5d4")));
    ASSERT_NE(e, std::find(b, e, Move("c5e3")));
    ASSERT_NE(e, std::find(b, e, Move("c5f2")));
    ASSERT_NE(e, std::find(b, e, Move("c5g1")));
    ASSERT_NE(e, std::find(b, e, Move("c5b4")));
    ASSERT_NE(e, std::find(b, e, Move("c5a3")));
    ASSERT_NE(e, std::find(b, e, Move("e4e5")));
    ASSERT_NE(e, std::find(b, e, Move("e4e6")));
    ASSERT_NE(e, std::find(b, e, Move("e4e7")));
    ASSERT_NE(e, std::find(b, e, Move("e4e3")));
    ASSERT_NE(e, std::find(b, e, Move("e4e2")));
    ASSERT_NE(e, std::find(b, e, Move("e4e1")));
    ASSERT_NE(e, std::find(b, e, Move("e4f4")));
    ASSERT_NE(e, std::find(b, e, Move("e4g4")));
    ASSERT_NE(e, std::find(b, e, Move("e4h4")));
    ASSERT_NE(e, std::find(b, e, Move("e4d4")));
    ASSERT_NE(e, std::find(b, e, Move("e4c4")));
    ASSERT_NE(e, std::find(b, e, Move("e4b4")));
    ASSERT_NE(e, std::find(b, e, Move("e4a4")));
    ASSERT_NE(e, std::find(b, e, Move("e4d5")));
    ASSERT_NE(e, std::find(b, e, Move("e4c6")));
    ASSERT_NE(e, std::find(b, e, Move("e4b7")));
    ASSERT_NE(e, std::find(b, e, Move("e4a8")));
    ASSERT_NE(e, std::find(b, e, Move("e4f5")));
    ASSERT_NE(e, std::find(b, e, Move("e4g6")));
    ASSERT_NE(e, std::find(b, e, Move("e4h7")));
    ASSERT_NE(e, std::find(b, e, Move("e4f3")));
    ASSERT_NE(e, std::find(b, e, Move("e4g2")));
    ASSERT_NE(e, std::find(b, e, Move("e4h1")));
    ASSERT_NE(e, std::find(b, e, Move("e4d3")));
    ASSERT_NE(e, std::find(b, e, Move("e4c2")));

    rules.applyMove("c5f2");
    ASSERT_EQ(Status::Playing, rules.status());
    ASSERT_EQ(38, rules.m_legal_moves.size());
    b = rules.m_legal_moves.begin();
    e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("e7e8")));
    ASSERT_NE(e, std::find(b, e, Move("e7e6")));
    ASSERT_NE(e, std::find(b, e, Move("e7e5")));
    ASSERT_NE(e, std::find(b, e, Move("e7e4")));
    ASSERT_NE(e, std::find(b, e, Move("e7f7")));
    ASSERT_NE(e, std::find(b, e, Move("e7g7")));
    ASSERT_NE(e, std::find(b, e, Move("e7h7")));
    ASSERT_NE(e, std::find(b, e, Move("e7d7")));
    ASSERT_NE(e, std::find(b, e, Move("e7c7")));
    ASSERT_NE(e, std::find(b, e, Move("e7b7")));
    ASSERT_NE(e, std::find(b, e, Move("e7a7")));
    ASSERT_NE(e, std::find(b, e, Move("e7d8")));
    ASSERT_NE(e, std::find(b, e, Move("e7f8")));
    ASSERT_NE(e, std::find(b, e, Move("e7f6")));
    ASSERT_NE(e, std::find(b, e, Move("e7g5")));
    ASSERT_NE(e, std::find(b, e, Move("e7h4")));
    ASSERT_NE(e, std::find(b, e, Move("e7d6")));
    ASSERT_NE(e, std::find(b, e, Move("e7c5")));
    ASSERT_NE(e, std::find(b, e, Move("e7b4")));
    ASSERT_NE(e, std::find(b, e, Move("e7a3")));
    ASSERT_NE(e, std::find(b, e, Move("c2c3")));
    ASSERT_NE(e, std::find(b, e, Move("c2c4")));
    ASSERT_NE(e, std::find(b, e, Move("c2c5")));
    ASSERT_NE(e, std::find(b, e, Move("c2c6")));
    ASSERT_NE(e, std::find(b, e, Move("c2c7")));
    ASSERT_NE(e, std::find(b, e, Move("c2c8")));
    ASSERT_NE(e, std::find(b, e, Move("c2c1")));
    ASSERT_NE(e, std::find(b, e, Move("c2d2")));
    ASSERT_NE(e, std::find(b, e, Move("c2e2")));
    ASSERT_NE(e, std::find(b, e, Move("c2f2")));
    ASSERT_NE(e, std::find(b, e, Move("c2b2")));
    ASSERT_NE(e, std::find(b, e, Move("c2a2")));
    ASSERT_NE(e, std::find(b, e, Move("c2b3")));
    ASSERT_NE(e, std::find(b, e, Move("c2a4")));
    ASSERT_NE(e, std::find(b, e, Move("c2d3")));
    ASSERT_NE(e, std::find(b, e, Move("c2e4")));
    ASSERT_NE(e, std::find(b, e, Move("c2d1")));
    ASSERT_NE(e, std::find(b, e, Move("c2b1")));
  }
}

TESTSUITE(KingMoves)
{
  TEST(BasicKingMoves)
  {
    Rules rules;
    ASSERT_EQ(true, rules.load("8/6k1/8/8/4K3/8/8/8 w - -"));

    ASSERT_EQ(Color::White, rules.m_side);
    ASSERT_EQ(WithKings, rules.hasNoKing);
    ASSERT_EQ(Status::Playing, rules.status());
    ASSERT_EQ(8, rules.m_legal_moves.size());
    std::vector<Move>::iterator b = rules.m_legal_moves.begin();
    std::vector<Move>::iterator e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("e4e5")));
    ASSERT_NE(e, std::find(b, e, Move("e4e3")));
    ASSERT_NE(e, std::find(b, e, Move("e4f4")));
    ASSERT_NE(e, std::find(b, e, Move("e4d4")));
    ASSERT_NE(e, std::find(b, e, Move("e4d5")));
    ASSERT_NE(e, std::find(b, e, Move("e4f5")));
    ASSERT_NE(e, std::find(b, e, Move("e4f3")));
    ASSERT_NE(e, std::find(b, e, Move("e4d3")));

    rules.applyMove("e4d5");
    ASSERT_EQ(Status::Playing, rules.status());
    ASSERT_EQ(8, rules.m_legal_moves.size());
    b = rules.m_legal_moves.begin();
    e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("g7g8")));
    ASSERT_NE(e, std::find(b, e, Move("g7g6")));
    ASSERT_NE(e, std::find(b, e, Move("g7h7")));
    ASSERT_NE(e, std::find(b, e, Move("g7f7")));
    ASSERT_NE(e, std::find(b, e, Move("g7f8")));
    ASSERT_NE(e, std::find(b, e, Move("g7h8")));
    ASSERT_NE(e, std::find(b, e, Move("g7h6")));
    ASSERT_NE(e, std::find(b, e, Move("g7f6")));
  }

  TEST(BlockedKingMoves)
  {
    Rules rules;
    ASSERT_EQ(true, rules.load("7k/8/8/3PP3/4KP2/4P3/8/8 w - -"));

    ASSERT_EQ(Color::White, rules.m_side);
    ASSERT_EQ(WithKings, rules.hasNoKing);
    ASSERT_EQ(Status::Playing, rules.status());
    std::vector<Move>::iterator b = rules.m_legal_moves.begin();
    std::vector<Move>::iterator e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("e4d4")));
    ASSERT_NE(e, std::find(b, e, Move("e4d3")));
    ASSERT_NE(e, std::find(b, e, Move("e4f3")));
    ASSERT_NE(e, std::find(b, e, Move("e4f5")));

    rules.applyMove("e4f5");
    ASSERT_EQ(Status::Playing, rules.status());
    ASSERT_EQ(3, rules.m_legal_moves.size());
    b = rules.m_legal_moves.begin();
    e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("h8g8")));
    ASSERT_NE(e, std::find(b, e, Move("h8h7")));
    ASSERT_NE(e, std::find(b, e, Move("h8g7")));
  }

  TEST(WhiteBlockedByCheck)
  {
    Rules rules;
    ASSERT_EQ(true, rules.load("7k/1b2rb2/8/8/n2K4/8/8/4n3 w - -"));

    ASSERT_EQ(Color::White, rules.m_side);
    ASSERT_EQ(WithKings, rules.hasNoKing);
    ASSERT_EQ(Status::Stalemate, rules.status());
    ASSERT_EQ(0, rules.m_legal_moves.size());
  }

  TEST(BlackBlockedByCheck)
  {
    Rules rules;
    ASSERT_EQ(true, rules.load("7K/1B2RB2/8/8/N2k4/8/8/4N3 b - -"));

    ASSERT_EQ(Color::Black, rules.m_side);
    ASSERT_EQ(WithKings, rules.hasNoKing);
    ASSERT_EQ(Status::Stalemate, rules.status());
    ASSERT_EQ(0, rules.m_legal_moves.size());
  }

  TEST(AllowedCastle)
  {
    Rules rules("r3k2r/8/8/8/8/8/8/R3K2R w KQkq -");

    ASSERT_EQ(Color::White, rules.m_side);
    ASSERT_EQ(WithKings, rules.hasNoKing);
    ASSERT_EQ(Status::Playing, rules.status());
    ASSERT_EQ(Castle::Both, rules.m_castle[Color::White]);
    ASSERT_EQ(Castle::Both, rules.m_castle[Color::Black]);

    std::vector<Move>::iterator b = rules.m_legal_moves.begin();
    std::vector<Move>::iterator e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("e1c1")));
    ASSERT_NE(e, std::find(b, e, Move("e1g1")));

    ASSERT_EQ(true, rules.isValidMove("e1g1"));
    ASSERT_EQ(true, rules.isValidMove("e1c1"));

    rules.applyMove("e1c1");
    /*ASSERT_EQ(NoPiece, rules.m_board[sqA1]);
    ASSERT_EQ(NoPiece, rules.m_board[sqE1]);
    ASSERT_EQ(WhiteKing, rules.m_board[sqC1]);
    ASSERT_EQ(WhiteRook, rules.m_board[sqD1]);

    b = rules.m_legal_moves.begin();
    e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("e8g8")));
    ASSERT_EQ(e, std::find(b, e, Move("e8c8"))); // Not allowed

    ASSERT_EQ(true, rules.isValidMove("e8g8"));
    ASSERT_EQ(false, rules.isValidMove("e8c8")); // Not allowed*/
  }

  TEST(NotAllowedCastle)
  {
  }

  TEST(TestInCheck)
  {

  }

  TEST(InitPos)
  {
    Rules rules;

    ASSERT_EQ(Color::White, rules.m_side);
    ASSERT_EQ(Status::Playing, rules.status());
    ASSERT_EQ(20, rules.m_legal_moves.size());

    rules.dispPseudoMoves();
    rules.dispLegalMoves();

    std::vector<Move>::iterator b = rules.m_legal_moves.begin();
    std::vector<Move>::iterator e = rules.m_legal_moves.end();
    ASSERT_NE(e, std::find(b, e, Move("a2a3")));
    ASSERT_NE(e, std::find(b, e, Move("a2a4")));
    ASSERT_NE(e, std::find(b, e, Move("b2b3")));
    ASSERT_NE(e, std::find(b, e, Move("b2b4")));
    ASSERT_NE(e, std::find(b, e, Move("c2c3")));
    ASSERT_NE(e, std::find(b, e, Move("c2c4")));
    ASSERT_NE(e, std::find(b, e, Move("d2d3")));
    ASSERT_NE(e, std::find(b, e, Move("d2d4")));
    ASSERT_NE(e, std::find(b, e, Move("e2e3")));
    ASSERT_NE(e, std::find(b, e, Move("e2e4")));
    ASSERT_NE(e, std::find(b, e, Move("f2f3")));
    ASSERT_NE(e, std::find(b, e, Move("f2f4")));
    ASSERT_NE(e, std::find(b, e, Move("g2g3")));
    ASSERT_NE(e, std::find(b, e, Move("g2g4")));
    ASSERT_NE(e, std::find(b, e, Move("h2h3")));
    ASSERT_NE(e, std::find(b, e, Move("h2h4")));
    ASSERT_NE(e, std::find(b, e, Move("b1c3")));
    ASSERT_NE(e, std::find(b, e, Move("b1a3")));
    ASSERT_NE(e, std::find(b, e, Move("g1h3")));
    ASSERT_NE(e, std::find(b, e, Move("g1f3")));

    ASSERT_EQ(true, rules.m_legal_moves == rules.m_pseudo_moves);
  }
}

TESTSUITE(ChessStatus)
{
  TEST(WhiteCheckMate)
  {
    Rules rules;
    ASSERT_EQ(true, rules.load("6k1/5ppp/8/8/8/8/8/R5K1 w k -"));
    ASSERT_EQ(Color::White, rules.m_side);
    ASSERT_EQ(Status::Playing, rules.status());
    ASSERT_EQ(true, rules.m_legal_moves.size() > 1);

    rules.applyMove("a1a8");
    ASSERT_EQ(Status::WhiteWon, rules.status());
    ASSERT_EQ(0, rules.m_legal_moves.size());
  }

  TEST(BlackCheckMate, DEADLINE_CPU_MS(3000))
    {
      Rules rules;
      ASSERT_EQ(true, rules.load("k2r4/4b3/8/8/8/8/PPP5/2K5 b - -"));
      ASSERT_EQ(Color::Black, rules.m_side);
      ASSERT_EQ(Status::Playing, rules.status());
      ASSERT_EQ(true, rules.m_legal_moves.size() > 1);

      rules.applyMove("e7g5");
      ASSERT_EQ(Status::Playing, rules.status());
      ASSERT_EQ(1, rules.m_legal_moves.size());
      std::vector<Move>::iterator b = rules.m_legal_moves.begin();
      std::vector<Move>::iterator e = rules.m_legal_moves.end();
      ASSERT_NE(e, std::find(b, e, Move("c1b1")));

      rules.applyMove("c1b1");
      rules.applyMove("d8d1");
      ASSERT_EQ(Status::BlackWon, rules.status());
      ASSERT_EQ(0, rules.m_legal_moves.size());
    }

  TEST(WhiteStalemate)
  {
    Rules rules;
    ASSERT_EQ(true, rules.load("2k5/2P5/3K4/8/8/8/8/8 w - -"));
    ASSERT_EQ(Color::White, rules.m_side);
    ASSERT_EQ(Status::Playing, rules.status());
    ASSERT_EQ(true, rules.m_legal_moves.size() > 1);

    rules.applyMove("d6c6");
    rules.generateValidMoves();
    ASSERT_EQ(Status::Stalemate, rules.status());
    ASSERT_EQ(0, rules.m_legal_moves.size());
  }

  TEST(BlackStalemate)
  {
    Rules rules;
    ASSERT_EQ(true, rules.load("8/8/8/8/8/5k2/6p1/6K1 b - -"));
    ASSERT_EQ(Color::Black, rules.m_side);
    ASSERT_EQ(Status::Playing, rules.status());
    ASSERT_EQ(true, rules.m_legal_moves.size() > 1);

    rules.applyMove("f3g3");
    rules.generateValidMoves();
    ASSERT_EQ(Status::Stalemate, rules.status());
    ASSERT_EQ(0, rules.m_legal_moves.size());
  }
}
