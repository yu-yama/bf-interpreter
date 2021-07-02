# bf-interpreter
BF interpreter written in C++

## Build using CMake
1. Run the followings at the root of this repository
```
mkdir build
cd build
cmake ..
make
```
1. An executable will be generated as `BFInterpreter` in the `build` directory

## How to
#### Read from file(s)
Pass bf source file(s) to the executable as command-line argument(s) (like `./BFInterpreter sample.bf`). Source files will be interpreted independently in the order of passing them when you give multiple filenames.

#### Read from `cin`
Run the executable with no command line arguments and pass a bf source to `cin`.

## License
Apache License Version 2.0. See `LICENSE` file for details.

Also, refer to `NOTICE` file for license and copyright notices of other projects that may be used.
