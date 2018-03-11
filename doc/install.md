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

## ChessNeuNeu Usage

```sh
./build/ChessNeuNeu --white <PLAYER> --black <PLAYER>
```

With <PLAYER>:
* `human` for human player
* `stockfish` for playing against Stockfish (need to be installed).
* `tscp` for playing against TSCP (need to be installed).
* `neuneu` for playing against our Neural Network IA.

If no parameters are given:
```sh
./build/ChessNeuNeu
```
will force human player to play Whites and Stockfish Blacks.