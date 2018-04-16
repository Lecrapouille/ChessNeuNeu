#include "Chess/Rules.hpp"
#include <crpcut.hpp>
#include <iostream>
#include <ostream>
#include <algorithm>

TESTSUITE(Position)
{
  // Load an initial chessboard
  TEST(RulesConstructor1)
  {
    Rules rules("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -");
    ASSERT_EQ(WithKings, rules.hasNoKing);
    ASSERT_EQ(true, Chessboard::Init == rules.m_board);
    ASSERT_EQ(255, rules.m_ep);
    // TODO: can castle
  }

  // Load an empty chessboard
  TEST(RulesConstructor2)
  {
    Rules rules("8/8/8/8/8/8/8/8 w - -");
    ASSERT_EQ(WithNoKings, rules.hasNoKing);
    ASSERT_EQ(true, Chessboard::Empty == rules.m_board);
    ASSERT_EQ(255, rules.m_ep);
    // TODO: can castle
  }

  // Load a chessboard from initial board and list of moves
  TEST(LoadPosition)
  {
    // Load empty chessboard
    Rules rules("8/8/8/8/8/8/8/8 w - -");
    rules.m_moved = "blah blah";

    // Play some moves from initial board
    std::string moves = "e2e4 e7e5 g1f3";
    rules.load(Chessboard::Init, moves);
    ASSERT_EQ(moves, rules.m_moved);

    // Load expected chessboard
    chessboard board;
    Color side;
    int ep;
    std::string FEN = "rnbqkbnr/pppp1ppp/8/4p3/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq -";
    ASSERT_EQ(true, import(FEN, board, side, ep));

    // Compare boards
    ASSERT_EQ(true, board == rules.m_board);
  }

  TEST(MoveBack1)
  {
    std::string moves0 = "";
    std::string moves1 = "e2e4";
    std::string moves2 = "e2e4 e7e5";
    std::string moves3 = "e2e4 e7e5 g1f3";

    Rules rules;
    rules.m_moved = "blah blah";
    rules.load(Chessboard::Init, moves3);
    ASSERT_EQ(moves3, rules.m_moved);

    rules.revertLastMove();
    ASSERT_EQ(moves2, rules.m_moved);

    rules.revertLastMove();
    ASSERT_EQ(moves1, rules.m_moved);

    rules.revertLastMove();
    ASSERT_EQ(moves0, rules.m_moved);

    ASSERT_EQ(true, Chessboard::Init == rules.m_board);
  }
}
