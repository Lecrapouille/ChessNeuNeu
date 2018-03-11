Not an effective chess engine but just a brainstorming/Proof of concept project for understanding how neural network can learn and play chess.

Project started after reading this article:
https://www.futura-sciences.com/tech/actualites/technologie-alphazero-ia-google-deepmind-devient-imbattable-echecs-61409/
which make interrogate ourself on how well NN can understand well chess strategies.

Read the following internal documentation:
* [archi](https://github.com/Lecrapouille/LEchecDeNeuneu/blob/master/doc/archi.md) for code source architecture.
* [credits](https://github.com/Lecrapouille/LEchecDeNeuneu/blob/master/doc/credits.md) for bibliography and code source inspirations.
* [install](https://github.com/Lecrapouille/LEchecDeNeuneu/blob/master/doc/install.md) for compilation and how to call options.

Some state of art documents are given in the doc/ directory.

Project features:
* Chessboard GUI and basic interactions with the user.
* Chess engine: chess rules implemented, basic eval functions and no alpha/beta implementation.
* Communication with Stockfish or tcsp.
* (in gestation) neural implementation.
