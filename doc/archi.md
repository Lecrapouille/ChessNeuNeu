# Code architecture

This document describes the architecture of the C++ code source given in the directory src/.
The following figures are generated with the [PlantUML](http://plantuml.com/) tool. Please refer to this [document](http://plantuml.com/class-diagram) for UML nomenclature.

## Class Diagram

![alt tag](https://github.com/Lecrapouille/LEchecDeNeuneu/blob/master/doc/UML/class.png)

The Application class uses the [SFML](https://www.sfml-dev.org/index-fr.php) library for drawing a 2D chessboard and managing
interactions with the user (mouse and keyboard events). The application mainly manages a stack of GUI classes (Graphical User
Interface) and call their private methods (as friend class). For simplifying let's say that GUI is a window that we push and
pop from each others. Each GUI draws its own window and manages its own mouse/keyboard events.

There are two GUI for our application:
* the chessboard for playing a chess game with the mouse (press and release mouse button),
* the pawn promotion for selecting the desired piece.
In the code source, a chess figure is a movable texture representing a chess piece.

The stack is used as very basic state machine:
* the chessboard pops up the promotion board when a pawn is promoting,
* the user clicks on the desired figures and the window is poped down and the chessboard is shown again.

Our chess game (ChessNeuNeu class) is an Application class containing a chessboard, creating different kind of players (human, IA, Stockfish)
and a class knowing the chess rules (Rules class).

The Rules class is both the chess referee and game states:
* game states: because are stored informations like chessboard with pieces information (position, color, type), king positions ...
* referee: because this class knows legal moves and prevents the player to play illegal moves.
* knows the past of the game: the list of played moves.
A chessboard can be loaded from a given position or by the list of moves from the initial position.

A Player class is an interface which returns the (legal) move to play. This information is given to the class Rules for updating its states and to
the Board for the display. Current implementor of the Player interface are:
* Human: the human moves figures on the chessboard (class Board).
* [Stockfish](https://github.com/mcostalba/Stockfish): is the best open source chess program. Thanks to the IPC class we can interact with it.
* [TCSP](http://www.tckerrigan.com/Chess/TSCP/): Tom Kerrigan’s Simple Chess Program, is another open source chess program. Thanks to the IPC class we can interact with it.
* NeuNeu: a "in-gestation" class for experementing neural network (NN). A specific document will be written explaining what we have done.

## Inter Process Communication

ChessNeuNeu can be connected to an external chess game like [Stockfish](https://github.com/mcostalba/Stockfish) or [TCSP](http://www.tckerrigan.com/Chess/TSCP/).
The class Inter Process Communication (IPC) is made for sending ChessNeuNeu moves to the external programm and also for listening to its move.
The IPC class is a bidirectional Unix pipe: reading stockfish moves and we sends to it our move.

![alt tag](https://github.com/Lecrapouille/LEchecDeNeuneu/blob/master/doc/UML/IPC.png)

Note that:
* Stockfish only accepts legal moves. It does not return error code.
* TSCP returns an error when we sent to it an illegal move.
* Therefore, it seems than Stockfish cannot be easily used for training the ChessNeuNeu's neural network.
* Help wanted fot clarifying this point. Anyway our class Rules is enough for training.
