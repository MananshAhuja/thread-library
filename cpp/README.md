# Thread Library C++ Interface

This folder adds a C++ interface to the existing C thread library. The original C files are kept unchanged.

The C++ classes are in `include/` and `src/`. `Thread::start` accepts functions or lambdas. The demo takes the thread count and increment count from standard input. `src/context_init.cpp` provides the C-linkage context initializer for this build with the required x86-64 Linux stack alignment.

## Build and run

Use 64-bit Ubuntu or another x86-64 Linux distribution with GCC, G++, and GNU Make. From the main thread-library project folder, run:

```sh
make -C cpp run
```

Enter two positive integers when prompted, for example `2 3`. The demo should finish with `Final counter: 6 (expected 6)`.

Use `make -C cpp clean` to remove generated build files.