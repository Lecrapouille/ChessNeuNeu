# ChessNeuNeu

Not an effective chess engine but just a brainstorming/proof of
concept project for understanding how neural network (NN) can learn
by itself the movement of figures.

This project has started after the reading of this article:
https://www.futura-sciences.com/tech/actualites/technologie-alphazero-ia-google-deepmind-devient-imbattable-echecs-61409/
which make interrogate ourself on how well NN can understand well chess strategies.

## Project features

Implemented:

* Hard coded chess rules for knowing legal moves. Can be used as
  supervizor for learning NN.
* Communication with some chess engines (Stockfish or TCSP). Other
  engines can be easily added.
* GUI: with basic interaction with the human player. GUI and Players
  are thread separated meaning the GUI is not blocked when player is
  computing the next move.
* Self learning piece movement in a empty chessboard.
* Forsyth-Edwards notation

In gestation:
* Neural stuffs. Julia code using Knet library for learning basic
  patterns like piece blocked by a piece of its same side. This code
  is not yet shared.
* Have to learn how to port it into C++ code

Currently not (yet) implemented:
* Hard coded chess rules: thirty rules detection. Have to create a NN
  for learnint his rule.
* Chess engine communication protocol: ChessNeuNeu does not read chess
  moves from the console. Therefore, it cannot yet be used as chess
  engine.
* Alpha/beta or minimax algorithm. Using NN for optimizing the cost
  like AlphaGo.
* No game scoring analysis.

## State of Art

* Documents are placed in the doc/StateOfArt/ directory but also
* https://github.com/ashudeep/ConvChess/blob/master/convchess.pdf

## Internal Documentation

ChessNeuNeu internal documents are placed in doc/*.ml:
* [How to compile the project](https://github.com/Lecrapouille/LEchecDeNeuneu/blob/master/doc/install.md).
* [Code source architecture](https://github.com/Lecrapouille/LEchecDeNeuneu/blob/master/doc/archi.md).
* [Credits](https://github.com/Lecrapouille/LEchecDeNeuneu/blob/master/doc/credits.md).

## Command-Line Usage

### Launch the project without arguments

```
./ChessNeuNeu
```

Will make human player play with White color against Stockfish with Black color.


### Launch the project without arguments

```
./ChessNeuNeu --white <player> --black <player> --fen <board>
```

Where different players are:
* `human` for letting play a human player through the interaction of the GUI board.
* `neuneu` for letting play the Neural Network player.
* `stockfish` for letting play Stockfish.
* `tcsp` for letting play TCSP.

Where:
* `board` is the board position using the Forsyth-Edwards
  notation. Use this https://lichess.org/editor for generating the
  input.

### Example

```
./ChessNeuNeu --white stockfish --black human --fen "4k3/8/8/8/8/8/4P3/4K3 w - -"
```
