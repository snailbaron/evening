# evening - An experimental events library

[![Build Status](https://travis-ci.org/snailbaron/evening.svg?branch=master)](https://travis-ci.org/snailbaron/evening)
[![Build status](https://ci.appveyor.com/api/projects/status/fkje9ddpvmdapx32/branch/master?svg=true)](https://ci.appveyor.com/project/snailbaron/evening/branch/master)

## Requirements

* Modern C++ compiler (the code is written with C++17 in mind).

## How to use

The easiest way to use *evening* is to copy the header directly into your
include directory:

    curl -fLo <your_project_include_dir>/evening.hpp --create-dirs \
        https://raw.githubusercontent.com/snailbaron/evening/master/include/evening.hpp

## Building tests

### Additional requirements for building tests

* CMake 3.8 or newer.

### Build instructions

The usual CMake stuff:
```
mkdir build && cd build
cmake ..
make
```
