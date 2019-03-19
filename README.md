# ChessNeuNeu

Not an effective chess engine but just a brainstorming/proof of concept project for understanding how neural network (NN) can learn and play chess (mainly the movement of figures).

This project has started after the reading of this article:
https://www.futura-sciences.com/tech/actualites/technologie-alphazero-ia-google-deepmind-devient-imbattable-echecs-61409/
which make us interrogate on how well NN can understand chess
strategies, what filters/patterns they really learnt for evaluating
the chessboard (cost function used in minimax/alphabeta pruning). It's
very difficult to evaluate what IA have really learnt. We know that
neural network can have errors, biases ... and for evalutating a
chessboard position, even if an IA does 10-30% of errors when
selecting the best move, it'll difficult for human to prove it
(especially with IA having such a huge ELO levels).

To simplify the problem, we can start asking us if an IA can simply
learn by themselves the figure movements. I guess that in AlphaZero
chess rules and figure movements is hardcoded and IA improves
minimax/alphabeta pruning. In this case even a 1% error is not
acceptable: the IA is not allowed to cheat when moving its figures. In
addition, knowing that smallest chess programs have 100 lines of codes
of C or Python (including board evaluation), it is interesting to know
how fastidious (in term of memory for storing the NN, in term of
training iterations) it is to create a neural network doing the
equivalent of 100 lines of code. A final question could be: is it
possible to generate this 100 lines of code from the neural network ?

## Project features

Implemented:
* Hard coded chess rules for knowing legal moves. Can be used as
  supervizor for learning NN.
* Communication with some chess engines (Stockfish or TCSP). Other
  engines can be easily added. They can be used as
  supervizor for learning NN but also for estimating the board position.
* GUI: with basic interaction with the human player. GUI and Players
  are thread separated meaning the GUI is not blocked when player is
  computing the next move.
* Self learning piece movement in a empty chessboard.
* Forsyth-Edwards notation

In gestation:
* Neural stuffs. Julia code using Knet library for learning basic
  patterns like piece blocked by a piece of its same side. This code
  is not yet shared.
* Have to convert Julia code to C++ code

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
