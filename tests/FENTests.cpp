#include "Chess/FEN.hpp"
#include <crpcut.hpp>
#include <iostream>
#include <ostream>
#include <algorithm>

TESTSUITE(ForsythEdwardsNotation)
{
  TEST(LoadInit)
  {
    chessboard board(Chessboard::Empty);
    Color color;
    int ep;

    ASSERT_EQ(true, load("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -", board, color, ep));
    ASSERT_EQ(Color::White, color);
    std::cout << board << std::endl;
    ASSERT_EQ(true, Chessboard::Init == board);
    ASSERT_EQ(-1, ep);
  }

  TEST(LoadEmpty)
  {
    chessboard board(Chessboard::Init);
    Color color;
    int ep;

    ASSERT_EQ(true, load("8/8/8/8/8/8/8/8 b - -", board, color, ep));
    ASSERT_EQ(Color::Black, color);
    ASSERT_EQ(true, Chessboard::Empty == board);
    ASSERT_EQ(-1, ep);
  }

  TEST(Load)
  {
    chessboard board(Chessboard::Init);
    Color color;
    int ep;

    ASSERT_EQ(true, load("6k1/5ppp/8/8/8/8/8/R5K1 b - -", board, color, ep));
    ASSERT_EQ(Color::Black, color);
    ASSERT_NE(true, Chessboard::Init == board);
    ASSERT_EQ(-1, ep);

    // Empty the board
    board[sqG8] = NoPiece;
    board[sqF7] = NoPiece;
    board[sqG7] = NoPiece;
    board[sqH7] = NoPiece;
    board[sqG1] = NoPiece;
    board[sqA1] = NoPiece;

    // Check board is empty
    ASSERT_EQ(true, Chessboard::Empty == board);
  }

  TEST(LoadEnPassant)
  {
    chessboard board(Chessboard::Init);
    Color color;
    int ep;

    ASSERT_EQ(true, load("rnbqkbnr/ppp1pppp/8/8/3pP3/8/PPPP1PPP/RNBQKBNR b KQkq e3", board, color, ep));
    ASSERT_EQ(Color::Black, color);
    ASSERT_EQ(sqE3, ep);
  }

#if 0
  TEST(LoadTooManyKnights)
  {
    chessboard board(Chessboard::Init);
    Color color;
    int ep;

    ASSERT_EQ(false, load("1knn3K/1n2n3/1n2n3/4n3/4n3/2nn4/2n4n/2nnnnn1 w - -", board, color, ep));
    ASSERT_EQ(Color::Black, color);
    ASSERT_EQ(sqE3, ep);
  }

  TEST(LoadFunnyBoard)
  {
    chessboard board(Chessboard::Init);
    Color color;
    int ep;

    ASSERT_EQ(false, load("qknn2K1/qn2n3/qn2n3/q3n3/q3n3/q1bb4/q1b4r/q1bbbbb1 w - -", board, color, ep));
    ASSERT_EQ(Color::Black, color);
    ASSERT_EQ(sqE3, ep);
  }

  TEST(LoadTooManyPieces)
  {
    chessboard board(Chessboard::Init);
    Color color;
    int ep;

    ASSERT_EQ(false, load("qknn2K1/qnp1n3/qn2n3/q3n3/q3n3/q1bb4/q1b4r/q1bbbbb1 w - -", board, color, ep));
    ASSERT_EQ(Color::Black, color);
    ASSERT_EQ(sqE3, ep);
  }
#endif

  // TODO: test castle

  TEST(BadFENFormat)
  {
    chessboard board;
    Color color;
    int ep;

    // Less than 8 columns:
    //                            error
    //                              v
    ASSERT_EQ(false, load("6k1/5ppp/7/8/8/8/8/R5K1 b - -", board, color, ep));

    // Less than 8 columns:
    //                    error
    //                      v
    ASSERT_EQ(false, load("6k/5ppp/8/8/8/8/8/R5K1 b - -", board, color, ep));

    // More than 8 columns:
    //                            error
    //                              v
    ASSERT_EQ(false, load("6k1/5ppp/9/8/8/8/8/R5K1 b - -", board, color, ep));

    // More than 8 columns:
    //                      error
    //                        v
    ASSERT_EQ(false, load("6k1r/5ppp/9/8/8/8/8/R5K1 b - -", board, color, ep));

    // More than 8 lines:
    //                                        error
    //                                          v
    ASSERT_EQ(false, load("6k1/5ppp/8/8/8/8/8/8/R5K1 b - -", board, color, ep));

    // Less than 8 lines:
    //                                        error
    //                                          v
    ASSERT_EQ(false, load("6k1/5ppp/8/8/8/8/8/8/ b - -", board, color, ep));

    // Bad char:
    //                                      error
    //                                        v
    ASSERT_EQ(false, load("6k1/5ppp/8/8/8/8/8/Z5K1 b - -", board, color, ep));

    // Bad char:
    //                                           error
    //                                             v
    ASSERT_EQ(false, load("6k1/5ppp/8/8/8/8/8/R5K1 B - -", board, color, ep));

    // Too short:
    //                                           error
    //                                             v
    ASSERT_EQ(false, load("6k1/5ppp/8/8/8/8/8/R5K1 ", board, color, ep));

    // Too short:
    //                                          error
    //                                            v
    ASSERT_EQ(false, load("6k1/5ppp/8/8/8/8/8/R5K1", board, color, ep));

    // Bad castle:
    //                                             error
    //                                               v
    ASSERT_EQ(false, load("6k1/5ppp/8/8/8/8/8/R5K1 b j -", board, color, ep));

    // Bad castle:
    //                                                 error
    //                                                   v
    ASSERT_EQ(false, load("6k1/5ppp/8/8/8/8/8/R5K1 b KQkqq -", board, color, ep));

    // Bad en passant:
    //                                                 error
    //                                                   v
    ASSERT_EQ(false, load("6k1/5ppp/8/8/8/8/8/R5K1 b KQkq", board, color, ep));


    // Bad en passant:
    //                                                  error
    //                                                    v
    ASSERT_EQ(false, load("6k1/5ppp/8/8/8/8/8/R5K1 b KQkq 3e", board, color, ep));

    // Bad en passant:
    //                                                   error
    //                                                     v
    ASSERT_EQ(false, load("6k1/5ppp/8/8/8/8/8/R5K1 b KQkq e", board, color, ep));

    // Bad en passant:
    //                                                   error
    //                                                     v
    ASSERT_EQ(false, load("6k1/5ppp/8/8/8/8/8/R5K1 b KQkq e9", board, color, ep));

    // Bad en passant:
    //                                                  error
    //                                                    v
    ASSERT_EQ(false, load("6k1/5ppp/8/8/8/8/8/R5K1 b KQkq H8", board, color, ep));

    // TODO: check number of pieces
  }
}
