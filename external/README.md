Add here all external libraries that ChessNeuNeu depends on and which cannot be installed directly with the OS installer (apt-get, brew ...):
* backward-cpp: allows to display the stack when a segfault occures.
* CmdParser: a library for parsing program options.

ChessNeuNeu does not use git submodules for external libs but two bash scripts for doing the same work.
These scripts have been separated to be shared by the ../Makefile and by continous integration scripts (../.integration/):
* download-external-libs.sh: this script will git clone (some recurisvely) needed libraries.
* compile-external-libs.sh: this script will compile these libraries.

These libraries will not be installed in your environement (no sudo make install calls are made). Therefore ChessNeuNeu Makefile has to know explicitly
the path of compiled resources (header files, static/shared libraries, ...).

Scripts take one parameter: the name of the architecture: `Linux` or `Darwin` or `Windows`.
When using Makefile the correct architecture value will be automaticly taken.
