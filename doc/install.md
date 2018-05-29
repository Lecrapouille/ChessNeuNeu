# Compilation, Usage and Installation Guide

## Prerequesite

Install:
* Stockfish: https://github.com/mcostalba/Stockfish
* SMFL: https://www.sfml-dev.org/index-fr.php
* (Optional) TCSP: http://www.tckerrigan.com/Chess/TSCP/

Stockfish and SMFL are available with your operating system installer (Linux/Debian: apt-get, OSX: homebrew):
* Linux: 
```sh
sudo apt-get install libsfml-dev stockfish
```
* OSX:
```sh
homebrew install sfml stockfish
```

TCSP shall be installed manually:
* download the zip http://www.tckerrigan.com/Chess/TSCP/
* For compiling it:
```sh
make
```

## Compile ChessNeuNeu

```sh
git clone https://github.com/Lecrapouille/LEchecDeNeuneu.git
cd LEchecDeNeuneu
make CmdParser
make -j8
```

A binary name ChessNeuNeu is created in the directory build/

## Command-Line Usage

### Launch the project without arguments

```
./ChessNeuNeu
```

Will make human player play with White color against Stockfish with Black color.


### Launch the project without arguments

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

### Example

```
./ChessNeuNeu --white stockfish --black human --fen "4k3/8/8/8/8/8/4P3/4K3 w - -"
```