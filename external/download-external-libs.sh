#!/bin/bash

### This script will git clone some libraries that ChessNeuNeu needs and
### compile them. To avoid pollution, they are not installed into your
### environement. Therefore ChessNeuNeu Makefiles have to know where to
### find their files (includes and static/shared libraries).

### $1 is given by ../Makefile and refers to the current architecture.
if [ "$1" == "" ]; then
  echo "Expected one argument. Select the architecture: Linux, Darwin or Windows"
  exit 1
fi
ARCHI="$1"
TARGET="$2"

### Delete all previous directories to be sure to have and compile
### fresh code source.
rm -fr CmdParser Loki 2> /dev/null

function cloning
{
    REPO="$1"
    shift

    echo -e "\033[35m*** Cloning: \033[36mhttps://github.com/$REPO\033[00m >= \033[33m$TARGET\033[00m"
    git clone https://github.com/$REPO.git --depth=1 $* > /dev/null
}

### Library CmdParser is a C++ alternative to C getopt for parsing options like --help
### License: MIT
cloning FlorianRappl/CmdParser

### C++17 chess engine
### License: GPL-3.0
cloning Lecrapouille/Loki
