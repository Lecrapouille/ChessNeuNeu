# Unit Tests

## Prerequisite

- Unit tests used the tool GoogleTest framework. Here how to install it (Linux and MacOS):

```sh
wget https://github.com/google/googletest/archive/release-1.11.0.tar.gz
tar xf release-1.11.0.tar.gz
cd googletest-release-1.11.0
cmake -DBUILD_SHARED_LIBS=OFF -DCMAKE_POSITION_INDEPENDENT_CODE=ON .
sudo make install
```

- Install gcovr for the code coverage report:
```sh
sudo apt-get install gcovr
```

## Compilation

```sh
cd ChessNeuNeu/tests/
make coverage -j8
```

A html report has been created in `doc/` and automatically opened.
