# winsh

A minimal Unix shell written in C++.

## Requirements

- CMake 3.25 or newer
- C++26 (tested with GCC 15.3)

## Build

```sh
cmake -S . -B build
cmake --build build
```

## Usage

```sh
./build/winsh
```

Built-ins are `cd` (supporting `cd -` and `~` expansion), `pwd`, and `exit`.
Any other command is resolved on `PATH` and executed as an external process.

## Status

Quoting, pipes, redirection, globbing, environment assignment, and job control are not yet implemented.

## License

See [LICENSE](LICENSE).
