# Code architecture

This document describes the architecture of the C++ code source given in the directory `src/`.
The following figures are generated with the [PlantUML](http://plantuml.com/) tool. Please refer to this [document](http://plantuml.com/class-diagram) for UML nomenclature.

![alt tag](https://github.com/Lecrapouille/LEchecDeNeuneu/blob/master/doc/UML/class.png)

## Graphical User Interface

This project uses the [SFML](https://www.sfml-dev.org/index-fr.php) library for drawing a basic GUI: a simple 2D
chessboard and managing interactions with the user (mouse and keyboard events). The class Application uses a
stack as a very basic state machine for organizing windows between them. The GUI class (Graphical User Interface)
is an interface for implementing methods needed for displaying a window and managing mouse and keyboard events.
The Application class does not participate to the windows management its goal is just to push and pop GUI class
and call their methods.

There are two GUI implementation for our application:
* the chessboard: for playing a chess game with the mouse (press and release mouse button).
* the pawn promotion for selecting the desired piece.

The GUI chessboard pops up a promotion GUI when a pawn is promoting. With the promotion GUI, when the user clicks
on the desired figures, this window is pop down and the chessboard is shown again.

Our chess game (ChessNeuNeu class) is class inheriting from the Application class, contains a chessboard, can create
different kind of players (human, IA, Stockfish) and a class knowing the chess rules (Rules class).

## Chess engine

The Rules class contains game states and is used as a chess referee:
* game states: because are stored states like chessboard with pieces information (position, color, type), king positions ...
* referee: because this class knows legal moves and prevents the player to play illegal moves.
* knows the past of the game: the list of played moves.
A chessboard can be loaded from a given position or by the list of moves from the initial position.

A Player class is an interface which has to return the valid move to play. This information is given to the class Rules for updating its states and to
the GUI Board for the display. Current implementor of the Player interface are:
* Human: the human player interacts with the GUI board for moving figures.
* NeuNeu: is a "in-gestation" class for experimenting neural network (NN). A specific document will be written explaining what we have done.
* [Stockfish](https://github.com/mcostalba/Stockfish): is the strongest open source chess program. Thanks to the IPC class ChessNeuNeu can interact with it.
* [TCSP](http://www.tckerrigan.com/Chess/TSCP/): Tom Kerrigan’s Simple Chess Program, is another open source chess program. Thanks to the IPC class ChessNeuNeu can interact with it.

## Inter Process Communication

ChessNeuNeu can be connected to an external chess game like [Stockfish](https://github.com/mcostalba/Stockfish) or [TCSP](http://www.tckerrigan.com/Chess/TSCP/).
The class Inter Process Communication (IPC) is made for sending ChessNeuNeu moves to the external program and also for listening to its move.
The IPC class is a bidirectional Unix pipe: reading stockfish moves and we sends to it our move.

![alt tag](https://github.com/Lecrapouille/LEchecDeNeuneu/blob/master/doc/UML/IPC.png)

Note that:
* Stockfish only accepts legal moves. It does not return error code.
* TSCP returns an error when we sent to it an illegal move.
* Therefore, it seems than Stockfish cannot be easily used for training the ChessNeuNeu's neural network.
* Help wanted for clarifying this point. Anyway our class Rules is enough for training.
