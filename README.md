# ChessNeuNeu

Not an effective chess engine but just a brainstorming/proof of concept
project for understanding how deep learning (such as neural network (NN)
or convolution neural network (CNN)) can learn chess rules and understand
chess movements without cheating.

This project has started after the reading of this article:
https://www.futura-sciences.com/tech/actualites/technologie-alphazero-ia-google-deepmind-devient-imbattable-echecs-61409/
which make us interrogate on how well machine learning can understand chess
strategies, what filters/patterns they really learnt for evaluating
the chessboard (cost function used in minimax/alphabeta pruning). It's
very difficult to evaluate what IA have really learnt. We know that
neural network can have errors, biases ... and for evalutating a
chessboard position, even if the machine does 10-30% of errors when
selecting the best move, it will difficult for a human to prove it
especially with IA having such a huge ELO levels.

To simplify the problem, we can start asking us if a machine can simply
learn by themselves chess rules. I guess that in AlphaZero chess rules
are hardcoded and the deep learning improved minimax/alphabeta pruning.
In other [https://github.com/ashudeep/ConvChess](projects) the machine can
predict the chess movements but have around 1% error on learning rules.
In this case even a 1% error is not acceptable because cheating when moving
its pieces is forbidden.

In addition, knowing that smallest chess engines have 100 lines of codes
of C or Python (including board evaluation) like
[https://github.com/thomasahle/sunfish](sunfish), it is interesting to know
how fastidious (in term of memory for storing the CNN, in term of
training iterations) it is to create a neural network doing the
equivalent of 100 lines of code. Finaly a question could be: is it
possible to generate this 100 lines of code from what have learnt the
neural network ?

## Table of Contents

* [How to compile the project](https://github.com/Lecrapouille/LEchecDeNeuneu/blob/master/doc/Compilation.md).
* [Command-Line](https://github.com/Lecrapouille/LEchecDeNeuneu/blob/master/doc/CommandLine.md).
* [Project features](https://github.com/Lecrapouille/LEchecDeNeuneu/blob/master/doc/ProjectFeatures.md).
* [Software Architecture](https://github.com/Lecrapouille/LEchecDeNeuneu/blob/master/doc/SoftwareArchitecture.md).
* [Chess Algebra](https://github.com/Lecrapouille/LEchecDeNeuneu/blob/master/doc/ChessAlgebra-fr.pdf). In french for the moment.
* [Machine Learning Algorithms](https://github.com/Lecrapouille/LEchecDeNeuneu/blob/master/doc/MachineLearning-en.pdf). Work in progress.
* [Credits](https://github.com/Lecrapouille/LEchecDeNeuneu/blob/master/doc/Credits.md).


