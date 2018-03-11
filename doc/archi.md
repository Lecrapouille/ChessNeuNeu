# Code architecture

This document describes the architecture of the C++ code source given in the directory src/.
The following figures are generated with the [PlantUML](http://plantuml.com/) tool. Please refer to this [document](http://plantuml.com/class-diagram) for UML nomenclature.

## Class Diagram

![alt tag](https://github.com/Lecrapouille/LEchecDeNeuneu/blob/master/doc/UML/class.png)

The GUI class uses the [SFML](https://www.sfml-dev.org/index-fr.php) for drawing a 2D chessboard and managing interactions with the user (mouse events).
The GUI mainly manages a stack of GUI states (GUIState class) and call their private methods (friend). For simplifying let's say that GUI state = a window. Each state is an interface for managing its own drawing and mouse/keyboard events.
There are two GUI states:
* the chessboard for playing a chess game,
* the pawn promotion for selecting the desired piece.
In the code source, a chess figure is a movable texture representing a chess piece.

A chess game (ChessGame class) is a GUI state containing a chessboard, two players and a class knowing the chess rules.

The Rules class is both the chess referee and the state of the game:
* referee because this class knows legal moves and prevents the player to play illegal moves.
* store the game states: the chessboard internal memory with pieces information: position, color, type. Position of kings ...
* knows the past of the game: the list of played moves.
A chessboard can loaded from a given position or by the list of moves from the initial position.

A player is an interface class which returns the (legal) move to play. This information is given to the class Rules for updating its states.
Current implementor of the Player interface are:
* Human: the human moves figures on the chessboard (class Board).
* [Stockfish](https://github.com/mcostalba/Stockfish): is the best open source chess program. Thanks to the IPC class we can interact with it.
* [TCSP](http://www.tckerrigan.com/Chess/TSCP/): Tom Kerrigan’s Simple Chess Program, is another open source chess program. Thanks to the IPC class we can interact with it.
* ChessNN: a class for experementing neural network (NN).

## Inter Process Communication

ChessNN can be connected to an external chess game like [Stockfish](https://github.com/mcostalba/Stockfish) or [TCSP](http://www.tckerrigan.com/Chess/TSCP/).
The class Inter Process Communication (IPC) is made for sending ChessNN moves to the external programm and also for listening to its move.
The IPC class is a bidirectional Unix pipe: reading stockfish moves and we sends to it our move.

![alt tag](https://github.com/Lecrapouille/LEchecDeNeuneu/blob/master/doc/UML/IPC.png)

Note that:
* Stockfish only accept legal moves. It does not return error code.
* TSCP return an error when sending to it illegal move.
* Therefore it seems than Stockfish cannot be used easily for training the ChessNN's neural network.
* Help wanted fot clarifying this point.
