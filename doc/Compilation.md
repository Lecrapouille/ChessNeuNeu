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
git clone https://github.com/Lecrapouille/ChessNeuNeu.git --recurse-submodules
cd ChessNeuNeu
make download-external-libs
make compile-external-libs
make -j8
```

Where 8 of `-j8` is to adapt to the number of your CPU cores.
A binary name `ChessNeuNeu` should have been created inside the directory `build/`
