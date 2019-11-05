# Software Architecture

This document describes the software architecture of this project located
in the directory `src/` and written in C++. The following class diagrams
have been generated with the [PlantUML](http://plantuml.com/) tool. Please
refer to this [document](http://plantuml.com/class-diagram) for UML nomenclature.
The whole architecture of ChessNeuNeu can be summarized by the following figure.

![alt tag](https://github.com/Lecrapouille/LEchecDeNeuneu/blob/master/doc/UML/class.png)

In this document, we are going to explain this diagram more in details.

## Graphical User Interface

The `GUI` class (Graphical User Interface) is an interface class for
implementing methods needed for displaying a window and managing mouse
and keyboard events. This project uses the
[SFML](https://www.sfml-dev.org/index-fr.php) library for drawing the
GUI. There are two `GUI` implementation for our application:
* a 2D chessboard (`guiBoard` class): managing interactions with the user
  (mouse and keyboard events) for letting it playing a chess game with
  the IA or Stockfish.
* the pawn promotion for selecting the desired promoted figure
  (`guiPromotion` class).

These `GUI` classes are controled by the `Application` class which uses
a stack of `GUI` as a very basic state machine for organizing windows
between them. `Application` class does not interact with the user actions.
Its goal is just to push and pop `GUI` instances and call their methods.
For example, the chessboard GUI `guiBoard` pops up a promotion GUI when a
pawn is promoting. From the promotion GUI `guiPromotion`, when the user
have clicked on the desired pieces, this window is poped down and the
chessboard is shown back.

Our main class game `ChessNeuNeu` is class inheriting from the
`Application` class and contains a class knowing the chess rules (`Rules`
class), a factory method for creating different kind of players
(human, IA, Stockfish).

## Chess engine

Note that an attempt of modeling chess movements in algebra is given in this
[document](https://github.com/Lecrapouille/LEchecDeNeuneu/blob/master/doc/ChessAlgebra-fr.pdf).
The Julia script have been implemented following formulas of this paper.

The `Rules` class contains the chess game states and is used as a
chess referee:
* game states: because are stored states such as the chessboard with
  the pieces information (position, color, type), king positions ...
* referee: because this class knows legal moves and prevents the
  human player to play illegal moves when he is clicking on the chess board.
* it knows the past of the game by managing the list of played moves
  (which allows to move back pieces).
* a chessboard can be loaded from a given position using the
  Forsyth-Edwards Notation (FEN) or by the list of moves from the initial
  position.

Note that the `Rules` class has been *hacked* to allow forbidden
situations like a chessboard without kings ... in the aim to teach the
IA (like moving a piece on an empty board).

A `Player` class is an interface class which plays a valid chess movement.
This move is given to the class `Rules` for updating its states and to the GUI Board for its display.
Current implementor of the `Player` interface are:
* NeuNeu: is a "in-gestation" IA class for experimenting machine learning
  (ML) algorithms. A [specific document](https://github.com/Lecrapouille/LEchecDeNeuneu/blob/master/doc/MachineLearning-en.pdf)
  explains used alogrithms. NeuNeu is a french joke for neural network (NN) where neuneu means foolish.
* [Stockfish](https://github.com/mcostalba/Stockfish): is the
  strongest open source chess program. Thanks to the `IPC` class
  ChessNeuNeu can interact with it (an maybe learn from it).
* [TCSP](http://www.tckerrigan.com/Chess/TSCP/): Tom Kerrigan’s Simple
  Chess Program, is another open source chess program. Thanks to the
  `IPC` class ChessNeuNeu can interact with it.
* Human: is a little more tricky : this class does not generate the move
  directly. Indeed the human player interacts with the GUI board for
  moving pieces and this last has for notifying his move. In addition,
  the player can play an illegal move which shall not be send to the opponent.
  The `Rules` class has to be used before to invalidate the movement.

A factory method in `ChessNeuNeu` can create concrete `Player` classes. 

## Inter Process Communication

ChessNeuNeu can be connected to an external chess game like
[Stockfish](https://github.com/mcostalba/Stockfish) or
[TCSP](http://www.tckerrigan.com/Chess/TSCP/). The class Inter
Process Communication (IPC) is made for sending ChessNeuNeu moves to
the external program and also for listening to its move. The IPC class
is a bidirectional Unix pipe: reading the chess move from the external
program (such as Stockfish) and sending to it the next move.

Note that for creating a pipe, the ChessNeuNeu process shall be forked and the 
child process shall call the execve() function which replace the current
process by the new one (in our case ChessNeuNeu's child process is replaced
by Stockfish or TCSP). The parent process stays the ChessNeuNeu process which can
communicate with Stockfish or TCSP through the pipe as soon as file descriptors (fd) of the
pipe have been to correctly connected to process (dup2) and unused fd closed (because
when forking fd are also duplicated and are common to parent and child process which
has undefined behavior, see [this document](http://www.zeitoun.net/articles/communication-par-tuyau/start)
for more informations).

`Player` class implementer such as `Stockfish` class and `TCSP` class uses `IPC` class.

![alt tag](https://github.com/Lecrapouille/LEchecDeNeuneu/blob/master/doc/UML/IPC.png)

Note that:
* Stockfish only accepts legal moves. It does not return error code.
* TSCP returns an error when we sent to it an illegal move.
* Therefore, it seems than Stockfish cannot be easily used for
  training the ChessNeuNeu's neural network.
* Help wanted for clarifying this point. Anyway our class Rules is
  enough for training.

## Machine Learning

This part is under progress. Here what is have been done:
* src/Players/NeuNeu.[ch]pp: (code to be cleaned) train all pieces to
  move on a empty chessboard. Once the training has been done, the
  movement is get randomly depending on its probabilty.
* scripts/ChessKnet.jl Julia script using the Knet API with the
  simplest convolutional network (CNN): training how to move a Rook on 8x1
  chessboard containing pieces of the same solor placed
  randomly and blocking its movement. The CNN learns the distance of the Rook to
  the first blocking piece. Note: the chessboard is not 8x8 but 8x1 in the aim to have the
  simplest neural network case.

This [document](https://github.com/Lecrapouille/LEchecDeNeuneu/blob/master/doc/MachineLearning-en.pdf)
explains in better algorithms.
