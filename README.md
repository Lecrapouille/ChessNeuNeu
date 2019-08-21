# ChessNeuNeu

Not an effective chess engine but just a brainstorming/proof of concept
project for understanding how deep learning (such as neural network (NN)
or convolution neural network (CNN)) can learn chess rules and understand
chess movements without cheating.

This project has started after the reading of this article:
https://www.futura-sciences.com/tech/actualites/technologie-alphazero-ia-google-deepmind-devient-imbattable-echecs-61409/
which make us interrogate on how well machine learning can understand chess
strategies, what filters/patterns they really learnt for evaluating
the chessboard (cost function used in minimax/alphabeta pruning). It's
very difficult to evaluate what IA have really learnt. We know that
neural network can have errors, biases ... and for evalutating a
chessboard position, even if the machine does 10-30% of errors when
selecting the best move, it will difficult for a human to prove it
especially with IA having such a huge ELO levels.

To simplify the problem, we can start asking us if a machine can simply
learn by themselves chess rules. I guess that in AlphaZero chess rules
are hardcoded and the deep learning improved minimax/alphabeta pruning.
In other [https://github.com/ashudeep/ConvChess](projects) the machine can
predict the chess movements but have around 1% error on learning rules.
In this case even a 1% error is not acceptable because cheating when moving
its pieces is forbidden.

In addition, knowing that smallest chess engines have 100 lines of codes
of C or Python (including board evaluation) like
[https://github.com/thomasahle/sunfish](sunfish), it is interesting to know
how fastidious (in term of memory for storing the CNN, in term of
training iterations) it is to create a neural network doing the
equivalent of 100 lines of code. Finaly a question could be: is it
possible to generate this 100 lines of code from what have learnt the
neural network ?

## Project features

Implemented:
* Hard coded chess rules for knowing legal moves on a given chessboard.
  Can be used as supervizor for teaching NN.
* Communication with some chess engines (Stockfish or TCSP). Other
  engines can be easily added. They can be used as
  supervizor for teaching NN like estimating the board position.
* GUI: with basic interaction with the human player. GUI and Players
  are thread separated meaning the GUI is not blocked when player is
  computing the next move.
* Self learning piece movement in a empty chessboard. Play random moves
  (no chessboard evaluation).
* Forsyth-Edwards notation for loading a given chessboard.

In gestation:
* Neural stuffs. Julia scripts using Knet library for learning basic
  patterns like piece blocked by a piece of its same side.
* Have to convert Julia script to C++ code (using [https://github.com/davisking/dlib](dlib) ?).
* Documentation about algorithms.

Not for this scope:
* No game scoring analysis, no Alpha/beta or minimax algorithm.
  No machine learning for optimizing the cost like AlphaGo.
* No chess engine communication protocol: ChessNeuNeu does not read chess
  moves from the console. Therefore, it cannot yet be used as chess
  engine.
* Hard coded chess rules: thirty rules detection. Can be nice to create
  a NN for learning this rule.

## State of Art

* Documents are placed in the doc/StateOfArt/ directory but also
* https://github.com/ashudeep/ConvChess/blob/master/convchess.pdf
* https://github.com/BarakOshri/ConvChess/blob/master/papers/ConvChess.pdf

## Internal Documentation

ChessNeuNeu internal documents are placed in doc/*.ml or doc/*.pdf:
* [How to compile the project](https://github.com/Lecrapouille/LEchecDeNeuneu/blob/master/doc/install.md).
* [Code source architecture](https://github.com/Lecrapouille/LEchecDeNeuneu/blob/master/doc/archi.md).
* [Chess algebra](https://github.com/Lecrapouille/LEchecDeNeuneu/blob/master/doc/Regles.pdf). In french for the moment.
* [Machine Learning](https://github.com/Lecrapouille/LEchecDeNeuneu/blob/master/doc/ChessNeuNeu.pdf). WIP.
* [Credits](https://github.com/Lecrapouille/LEchecDeNeuneu/blob/master/doc/credits.md).

## Command-Line Usage

### Launch the project without arguments

```
./ChessNeuNeu
```

Will make human player play with White color against Stockfish with Black color.


### Launch the project with arguments

```
./ChessNeuNeu --white <player> --black <player> [--fen <board>]
```

Where different players are:
* `human` for letting play a human player through the interaction of the GUI board.
* `neuneu` for letting play the Neural Network player.
* `stockfish` for letting play [https://github.com/official-stockfish/Stockfish](Stockfish).
* `tcsp` for letting play [http://www.tckerrigan.com/Chess/TSCP/](TCSP).

Note: `--white` and `--black` can be shorten by `-w` and `-b`.

Optional:
* `board` is the board position using the Forsyth-Edwards
  notation. Use this https://lichess.org/editor for generating the
  input.

### Example

```
./ChessNeuNeu --white stockfish --black human --fen "4k3/8/8/8/8/8/4P3/4K3 w - -"
```
