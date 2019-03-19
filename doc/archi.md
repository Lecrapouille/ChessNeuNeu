# Code architecture

This document describes the architecture of the C++ code source given
in the directory `src/`. The following figures are generated with the
[PlantUML](http://plantuml.com/) tool. Please refer to this
[document](http://plantuml.com/class-diagram) for UML nomenclature.

The whole architecture of ChessNeuNeu can be summarized by this figure
that we are going to explain.

![alt tag](https://github.com/Lecrapouille/LEchecDeNeuneu/blob/master/doc/UML/class.png)

## Graphical User Interface

The `GUI` class (Graphical User Interface) is an interface for
implementing methods needed for displaying a window and managing mouse
and keyboard events. This project uses the
[SFML](https://www.sfml-dev.org/index-fr.php) library for drawing the
GUI. There are two `GUI` implementation for our application:
* a 2D chessboard (`guiBoard`): managing interactions with the user
  (mouse and keyboard events) for letting it playing a chess game with
  the IA or Stockfish.
* the pawn promotion for selecting the desired promoted figure
  (`guiPromotion`).

The `Application` class uses a stack of `GUI` as a very basic state
machine for organizing windows between them.  This class does not
participate to the windows management, its goal is just to push and
pop GUI instances and call their methods. The GUI chessboard pops up a
promotion GUI when a pawn is promoting. With the promotion GUI, when
the user clicks on the desired figures, this window is pop down and
the chessboard is shown again.

Our main class game `ChessNeuNeu` is class inheriting from the
`Application` class, contains a class knowing the chess rules (`Rules`
class), a factory method for creating different kind of players
(human, IA, Stockfish).

## Chess engine

The `Rules` class contains the chess game states and is used as a
chess referee:
* game states: because are stored states such as the chessboard with
  the figures information (position, color, type), king positions ...
* referee: because this class knows legal moves and prevents the
  player to play illegal moves.
* it knows the past of the game by managing the list of played moves
  (which allows to move back figures).
* a chessboard can be loaded from a given position (using the
  Forsyth-Edwards notation) or by the list of moves from the initial
  position.

Note that the `Rules` class has been "hacked" to allow forbidden
situations like a chessboard without kings ... in the aim to teach the
IA (like moving a piece on an empty board).

A `Player` class is an interface which returns the valid move to
play. This information is given to the class `Rules` for updating its
states and to the GUI Board for its display. A factory method can
create them. Current implementor of the `Player` interface are:
* NeuNeu: is a "in-gestation" class for experimenting neural network
  (NN). A specific document will be written explaining what we have
  done.
* [Stockfish](https://github.com/mcostalba/Stockfish): is the
  strongest open source chess program. Thanks to the IPC class
  ChessNeuNeu can interact with it.
* [TCSP](http://www.tckerrigan.com/Chess/TSCP/): Tom Kerrigan’s Simple
  Chess Program, is another open source chess program. Thanks to the
  IPC class ChessNeuNeu can interact with it.
* Human: is a little more tricky : this does not generate the move
  directly. Indeed the human player interacts with the GUI board for
  moving figures and this last has to notify it which move has been
  made. In addition the player can play an illegal move which shall
  not be send to the opponent. The `Rules` class has to be used before
  to invalidate the movement.

## Inter Process Communication

ChessNeuNeu can be connected to an external chess game like
[Stockfish](https://github.com/mcostalba/Stockfish) or
[TCSP](http://www.tckerrigan.com/Chess/TSCP/).  The class Inter
Process Communication (IPC) is made for sending ChessNeuNeu moves to
the external program and also for listening to its move. The IPC class
is a bidirectional Unix pipe: reading the chess move from the external
program (such as Stockfish) and send to it the next move.

`Player` class implementer such as `Stockfish` class and `TCSP` class uses it.

![alt tag](https://github.com/Lecrapouille/LEchecDeNeuneu/blob/master/doc/UML/IPC.png)

Note that:
* Stockfish only accepts legal moves. It does not return error code.
* TSCP returns an error when we sent to it an illegal move.
* Therefore, it seems than Stockfish cannot be easily used for
  training the ChessNeuNeu's neural network.
* Help wanted for clarifying this point. Anyway our class Rules is
  enough for training.

## Neural Network

This part is under progress. Here what is have been done:
* src/Players/NeuNeu.[ch]pp: (code to be cleaned) train all figures to
  move on a empty chessboard. Once the training has been done, the
  movement is get randomly depending on its probabilty.
* scripts/ChessKnet.jl Julia script using the Knet API with the
  simplest neural network: training how to move a rook on 8x1
  chessboard containing a figure of the same solor placed
  randomly. The chessboard is not 8x8 but 8x1 in the aim to have the
  simplest neural network case.
