# Command-Line Usage

## Launch the project without arguments

```
./ChessNeuNeu
```

Will make human player play as White color against the neural network as Black color.


## Launch the project with arguments

```
./ChessNeuNeu --white <player> --black <player> [--fen <board>]
```

Where different players are:
* `human` for letting play a human player through the interaction of the GUI board.
* `neuneu` for letting play the Neural Network player.
* `loki` for letting play [https://github.com/BimmerBass/Loki](Loki) (present when compiling this project).
* `stockfish` for letting play [https://github.com/official-stockfish/Stockfish](Stockfish) (need to be installed).
* `tcsp` for letting play [http://www.tckerrigan.com/Chess/TSCP/](TCSP) (need to be compiled and installed).

Implicit options will make human player play as White color against the neural network as Black color.

Optional:
* `board` is the board position using the Forsyth-Edwards
  notation. Use this https://lichess.org/editor for generating the
  input.

## Command-Line Example

```
./ChessNeuNeu --white stockfish --black human --fen "4k3/8/8/8/8/8/4P3/4K3 w - -"
```
