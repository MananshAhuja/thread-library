# Thread Library

A collaborative C thread library project with a C++ wrapper.

## Project status

This repository is the initial project scaffold. All requested headers, source
files, examples, tests, and benchmarks are present with TODO placeholders.
Thread creation, scheduling, and synchronization are not implemented yet.
The static archives build, but do not expose a working thread API.
Example, test, and benchmark placeholders print `NOT IMPLEMENTED` and exit
with code 77. Successful compilation is not a functional test pass.

## Team branches

| Contributor | Branch |
| --- | --- |
| Manansh | `Manansh` (default) |
| Aman | `Aman` |
| Aryan | `Aryan` |
| Aditya | `Aditya` |

All four branches start with the same project scaffold. Branch names are
case-sensitive. There is no additional `main` branch.

```sh
git clone https://github.com/MananshAhuja/thread-library.git
cd thread-library
git switch Aman  # Replace with your own branch name.
```

Commit and push changes to your branch, then open a pull request into `Manansh`
when ready to integrate. Each teammate needs collaborator write access to push;
creating a named branch does not grant account access.

## Build

Requires Make, a C11 compiler, a C++17 compiler, and `ar`.

```sh
make
make examples
make tests
make benchmarks
make clean
```

`make tests` compiles the test programs; implement assertions before running
these as a test suite. Generated files stay under `build/` and are ignored by
Git. `build/.gitkeep` preserves that otherwise-empty directory on GitHub.

## Structure

```text
thread-library/
├── benchmarks/
│   ├── context_switch_time.c
│   ├── creation_time.c
│   └── mutex_performance.c
├── build/
│   └── .gitkeep
├── examples/
│   ├── cpp_example.cpp
│   ├── hello_thread.c
│   ├── multiple_threads.c
│   ├── producer_consumer.c
│   └── shared_counter.c
├── include/
│   ├── mutex.h
│   ├── scheduler.h
│   ├── sync.h
│   ├── thread.h
│   └── thread.hpp
├── internal/
│   ├── scheduler_internal.h
│   ├── structures.h
│   └── thread_internal.h
├── src/
│   ├── cpp/
│   │   └── thread.cpp
│   ├── scheduler/
│   │   ├── priority_queue.c
│   │   ├── round_robin.c
│   │   └── scheduler.c
│   ├── sync/
│   │   ├── futex.c
│   │   ├── mutex.c
│   │   ├── semaphore.c
│   │   └── spinlock.c
│   └── thread/
│       ├── thread.c
│       ├── thread_create.c
│       ├── thread_exit.c
│       └── thread_join.c
├── tests/
│   ├── stress_test.c
│   ├── test_mutex.c
│   ├── test_scheduler.c
│   ├── test_thread_create.c
│   └── test_thread_join.c
├── .gitignore
├── Makefile
└── README.md
```
