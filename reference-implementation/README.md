# Thread library demonstration

This copy contains the source from `threadlibrary 5` unchanged, plus build and test files.
It implements cooperative, single OS thread (M:1) scheduling: workers switch only when
they yield, block, or exit. It does not preempt a worker that never yields.

## Build and run

Requires **Linux x86-64** with GCC or Clang, `make`, and a C++ compiler for the optional C++ demo.
The supplied `context_switch.S` uses the Linux x86-64 ABI and cannot build on Apple Silicon macOS.

```sh
make
make test
./thread_demo
./cpp_demo
```

`sync_test` compares sequential calls without the library against cooperative workers
with mutex protected increments and semaphore producer/consumer transfers. It exits
nonzero on a failed invariant. `switch_benchmark` compares sequential loop time with
yielding worker time using `CLOCK_MONOTONIC`, and reports elapsed time divided by the
library's recorded switch count. That average includes scheduling and work; it is
**not** an isolated hardware context switch measurement.

The tests exercise normal operation, not every deadlock or error path. The existing
implementation may retain terminated thread stacks on direct thread-to-thread exits.
