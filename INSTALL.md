# Prerequesites

Install SFML library:

* Linux: sudo apt-get install libsfml-dev
* OSX: homebrew install sfml

Install Stockfish:

* Linux: sudo apt-get install stockfish
* OSX: homebrew install stockfish

Install tscp:
download the zip http://www.tckerrigan.com/Chess/TSCP/
type: `make` for compiling

# Compiling NeuNeu

Copy https://github.com/FlorianRappl/CmdParser/blob/master/cmdparser.hpp in src/
type: `make` for compiling

A binary name NeuNeu is created in build/

# Running NeuNeu

./build/NeuNeu --white xxx --black yyy

Where xxx and yyy can be:

* human for human player
* neuneu for playing with my chess engine (in development)
* stockfish for playing with stockfish (if installed)
* tscp for playing with tscp (if installed)

