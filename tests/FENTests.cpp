#include "Chess/Rules.hpp"
#include <crpcut.hpp>
#include <iostream>
#include <ostream>
#include <algorithm>

TESTSUITE(ForsythEdwardsNotation)
{
  TEST(LoadInit)
  {
    Rules rules(Chessboard::Empty);

    ASSERT_EQ(true, rules.load("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -"));
    ASSERT_EQ(Color::White, rules.m_side);
    std::cout << rules.m_board << std::endl;
    ASSERT_EQ(true, Chessboard::Init == rules.m_board);
    ASSERT_EQ(Square::OOB, rules.m_ep);
    ASSERT_EQ(Castle::Both, rules.m_castle[Color::White]);
    ASSERT_EQ(Castle::Both, rules.m_castle[Color::Black]);
  }

  TEST(LoadEmpty)
  {
    Rules rules(Chessboard::Init);

    ASSERT_EQ(true, rules.load("8/8/8/8/8/8/8/8 b - -"));
    ASSERT_EQ(Color::Black, rules.m_side);
    ASSERT_EQ(true, Chessboard::Empty == rules.m_board);
    ASSERT_EQ(Square::OOB, rules.m_ep);
    ASSERT_EQ(Castle::NoCastle, rules.m_castle[Color::White]);
    ASSERT_EQ(Castle::NoCastle, rules.m_castle[Color::Black]);
  }

  TEST(Load)
  {
    Rules rules(Chessboard::Init);

    ASSERT_EQ(true, rules.load("6k1/5ppp/8/8/8/8/8/R5K1 b - -"));
    ASSERT_EQ(Color::Black, rules.m_side);
    ASSERT_NE(true, Chessboard::Init == rules.m_board);
    ASSERT_EQ(Square::OOB, rules.m_ep);
    ASSERT_EQ(Castle::NoCastle, rules.m_castle[Color::White]);
    ASSERT_EQ(Castle::NoCastle, rules.m_castle[Color::Black]);

    // Empty the rules.m_board
    rules.m_board[sqG8] = NoPiece;
    rules.m_board[sqF7] = NoPiece;
    rules.m_board[sqG7] = NoPiece;
    rules.m_board[sqH7] = NoPiece;
    rules.m_board[sqG1] = NoPiece;
    rules.m_board[sqA1] = NoPiece;

    // Check rules.m_board is empty
    ASSERT_EQ(true, Chessboard::Empty == rules.m_board);
  }

  TEST(LoadEnPassant)
  {
    Rules rules(Chessboard::Init);

    ASSERT_EQ(true, rules.load("rnbqkbnr/ppp1pppp/8/8/3pP3/8/PPPP1PPP/RNBQKBNR b KQkq e3"));
    ASSERT_EQ(Color::Black, rules.m_side);
    ASSERT_EQ(sqE3, rules.m_ep);
    ASSERT_EQ(Castle::Both, rules.m_castle[Color::White]);
    ASSERT_EQ(Castle::Both, rules.m_castle[Color::Black]);
  }

#if 0
  TEST(LoadTooManyKnights)
  {
    Rules rules(Chessboard::Init);

    ASSERT_EQ(false, rules.load("1knn3K/1n2n3/1n2n3/4n3/4n3/2nn4/2n4n/2nnnnn1 w - -"));
    ASSERT_EQ(Color::Black, rules.m_side);
    ASSERT_EQ(sqE3, rules.m_ep);
  }

  TEST(LoadFunnyBoard)
  {
    Rules rules(Chessboard::Init);

    ASSERT_EQ(false, rules.load("qknn2K1/qn2n3/qn2n3/q3n3/q3n3/q1bb4/q1b4r/q1bbbbb1 w - -"));
    ASSERT_EQ(Color::Black, rules.m_side);
    ASSERT_EQ(sqE3, rules.m_ep);
  }

  TEST(LoadTooManyPieces)
  {
    Rules rules(Chessboard::Init);

    ASSERT_EQ(false, rules.load("qknn2K1/qnp1n3/qn2n3/q3n3/q3n3/q1bb4/q1b4r/q1bbbbb1 w - -"));
    ASSERT_EQ(Color::Black, rules.m_side);
    ASSERT_EQ(sqE3, rules.m_ep);
  }
#endif

  // TODO: test castle

  // Good FEN: "6k1/5ppp/8/8/8/8/8/R5K1 b - -"
  TEST(BadFENFormat)
  {
    Rules rules;

    ASSERT_EQ(false, rules.load(""));

    // Not enough '/':
    //                                              error
    //                                                v
    ASSERT_EQ(false, rules.load("6k1/5ppp/8/8/8/8/R5K1 b - -"));

    // space delimiter:
    //                                error
    //                                  v
    ASSERT_EQ(false, rules.load("6k1/5pp p/8/8/8/8/R5K1 b - -"));


    // Less than 8 columns:
    //                                  error
    //                                    v
    ASSERT_EQ(false, rules.load("6k1/5ppp/7/8/8/8/8/R5K1 b - -"));

    // Less than 8 columns:
    //                           error
    //                             v
    ASSERT_EQ(false, rules.load("6k/5ppp/8/8/8/8/8/R5K1 b - -"));

    // More than 8 columns:
    //                                  error
    //                                    v
    ASSERT_EQ(false, rules.load("6k1/5ppp/9/8/8/8/8/R5K1 b - -"));

    // More than 8 columns:
    //                            error
    //                              v
    ASSERT_EQ(false, rules.load("6k1r/5ppp/9/8/8/8/8/R5K1 b - -"));

    // More than 8 lines:
    //                                        error
    //                                          v
    ASSERT_EQ(false, rules.load("6k1/5ppp/8/8/8/8/8/8/R5K1 b - -"));

    // Less than 8 lines:
    //                                        error
    //                                          v
    ASSERT_EQ(false, rules.load("6k1/5ppp/8/8/8/8/8/8/ b - -"));

    // Bad char:
    //                                      error
    //                                        v
    ASSERT_EQ(false, rules.load("6k1/5ppp/8/8/8/8/8/Z5K1 b - -"));

    // Bad char:
    //                                           error
    //                                             v
    ASSERT_EQ(false, rules.load("6k1/5ppp/8/8/8/8/8/R5K1 B - -"));

    // Too short:
    //                                                 error
    //                                                   v
    ASSERT_EQ(false, rules.load("6k1/5ppp/8/8/8/8/8/R5K1 "));

    // Too short:
    //                                                error
    //                                                  v
    ASSERT_EQ(false, rules.load("6k1/5ppp/8/8/8/8/8/R5K1"));

    // Bad castle:
    //                                                 error
    //                                                   v
    ASSERT_EQ(false, rules.load("6k1/5ppp/8/8/8/8/8/R5K1 b j -"));

    // Bad castle:
    //                                                 error
    //                                                   v
    ASSERT_EQ(false, rules.load("6k1/5ppp/8/8/8/8/8/R5K1 b KQkqq -"));

    // Bad en passant:
    //                                                 error
    //                                                   v
    ASSERT_EQ(false, rules.load("6k1/5ppp/8/8/8/8/8/R5K1 b KQkq"));


    // Bad en passant:
    //                                                  error
    //                                                    v
    ASSERT_EQ(false, rules.load("6k1/5ppp/8/8/8/8/8/R5K1 b KQkq 3e"));

    // Bad en passant:
    //                                                   error
    //                                                     v
    ASSERT_EQ(false, rules.load("6k1/5ppp/8/8/8/8/8/R5K1 b KQkq e"));

    // Bad en passant:
    //                                                         error
    //                                                           v
    ASSERT_EQ(false, rules.load("6k1/5ppp/8/8/8/8/8/R5K1 b KQkq e9"));

    // Bad en passant:
    //                                                        error
    //                                                          v
    ASSERT_EQ(false, rules.load("6k1/5ppp/8/8/8/8/8/R5K1 b KQkq H8"));

    // Too many kings:
    //                           error
    //                             v
    ASSERT_EQ(false, rules.load("6kk/5ppp/8/8/8/8/8/R5K1 b - -"));

    // Too many kings:
    //                           error               error
    //                             v                   v
    ASSERT_EQ(false, rules.load("6kk/5ppp/8/8/8/8/8/R5KK b - -"));

    // Not enough kings:
    //                         error
    //                           v
    ASSERT_EQ(false, rules.load("8/5ppp/8/8/8/8/8/R5K1 b - -"));

    // TODO: check number of pieces
  }
}
