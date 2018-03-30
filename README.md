# ChessNeuNeu

Not an effective chess engine but just a brainstorming/Proof of concept project for understanding how neural network (NN) can learn and play chess.

This project has started after the reading of this article:
https://www.futura-sciences.com/tech/actualites/technologie-alphazero-ia-google-deepmind-devient-imbattable-echecs-61409/
which make interrogate ourself on how well NN can understand well chess strategies.

## Project features

Implemented:
* Chess engine: knows legal moves.
* Communication with chess engine like Stockfish or TCSP.
* GUI: with basic interaction with the human player.
* Self learning piece movement in a empty chessboard.
* Forsyth-Edwards notation

In gestation:
* Neural stuffs.

Currently not implemented:
* Chess engine communication protocol: ChessNeuNeu does not yet read chess moves from the console. Therefore, it cannot be used as chess engine.
* Alpha/beta or minimax algorithm.
* No game scoring analysis.
* Thirty rules detection.

## State of Art

* Documents are placed in the doc/StateOfArt/ directory but also
* https://github.com/ashudeep/ConvChess/blob/master/convchess.pdf

## Internal Documentation

ChessNeuNeu internal documents are placed in doc/*.ml:
* [How to compile the project](https://github.com/Lecrapouille/LEchecDeNeuneu/blob/master/doc/install.md).
* [Code source architecture](https://github.com/Lecrapouille/LEchecDeNeuneu/blob/master/doc/archi.md).
* [Credits](https://github.com/Lecrapouille/LEchecDeNeuneu/blob/master/doc/credits.md).

## Options

Launch the project with the following arguments:

```
./ChessNeuNeu
```

Will make human player play with White color against Stockfish with Black color.

```
./ChessNeuNeu --white <player> --black <player> --fen <board>
```

Where different players are:
* `human` for letting a human player plays through the interaction of a GUI board.
* `neuneu` for letting the Neural Network plays.
* `stockfish` for letting Stockfish plays.
* `tcsp` for letting TCSP plays.

Where:
* `board` is the board position using the Forsyth-Edwards notation. Use this [https://lichess.org/editor](link) for generating this input.
