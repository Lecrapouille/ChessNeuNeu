# Unit Tests

## Prerequisite

Unit tests used the tool crpcut. Here how to install it:

### Ubuntu

```sh
sudo add-apt-repository ppa:jonlar/crpcut
sudo apt-get update
sudo apt-get install crpcut crpcut-dev
```

### From code source

Download sources from http://crpcut.sourceforge.net/ and untar it.

```sh
cd crpcut-1.9.4
mkdir build
cd build
cmake ..
make
sudo make install
```

CMake creates a file named `install_manifest.txt` indicating where it has installed files (including shared libraries). Be sure to have your shell environement variable `LD_LIBRARY_PATH` set to the location of the installed librairies. For example, in my case (but probably different from your environement")
```sh
export LD_LIBRARY_PATH=/usr/local/lib/
```

Else, when launching your test, crpcut will throw error that libcrpcut.so was not found and cannot be loaded.

## Compilation

```sh
cd LEchecDeNeuneu/tests/
make && ./build/ChessNeuNeu-UnitTest
```

With code coveraging. Install gcovr before:
```sh
sudo apt-get install gcovr
```

Launch code coverage:
```sh
make coverage
```

A html report has been created in doc/ and automaticly opened.
