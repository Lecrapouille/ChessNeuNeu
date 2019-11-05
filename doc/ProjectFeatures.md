# Project features

### Implemented

* Hard coded chess rules for knowing legal moves on a given chessboard.
  Can be used as supervizor for teaching the machine learning algorithms (ML).
* Communication with some chess engines (Stockfish or TCSP). Other
  engines can be easily added. They can be used as
  supervizor for training ML to estimate the board position.
* GUI: with basic interaction with the human player. GUI and Players
  are thread separated meaning the GUI is not blocked when player is
  computing the next move.
* Self learning piece movement in a empty chessboard. Play random moves
  (no chessboard evaluation).
* Forsyth-Edwards notation for loading a given chessboard.

### In gestation

* Neural stuffs. Julia scripts using Knet library for learning basic
  patterns like piece blocked by a piece of its same side.
* Have to convert Julia script to C++ code (using [https://github.com/davisking/dlib](dlib) ?).
* Documentation about algorithms.

### Not for this scope

* No game scoring analysis, no Alpha/beta or minimax algorithm.
  No machine learning for optimizing the cost like AlphaGo.
* No chess engine communication protocol: ChessNeuNeu does not read chess
  moves from the console. Therefore, it cannot yet be used as chess
  engine.
* Hard coded chess rules: thirty rules detection. Can be nice to create
  a ML for learning this rule.
