# Thread Library

A collaborative C thread library project with a C++ wrapper.

## Project status

This repository contains starter declarations using the naming scheme in the
team's reference code. Implementations, examples, tests, and benchmarks are
not yet available. Source files contain only includes; example, test, and
benchmark files are empty.

The declared interfaces use `thread_control_block`, `thread_state_t`,
`context_t`, `thread_create`, `thread_join`, `thread_exit`, `thread_yield`,
`thread_start`, `scheduler_next`, `context_init`, and `context_switch`.
Global thread state is declared in `internal/thread_internal.h`.
`internal/structure.h` uses the reference's singular filename, and
`internal/context.h` contains its x86-64 register layout.

`context_init` takes a stack pointer, matching the reference implementation.
Headers have include guards and C++ compatibility. Mutex and synchronization
headers have no API yet, as the corresponding reference files are empty.
The C++ header includes the C API; a C++ wrapper remains to be designed.

## Branches

`main` is the default integration branch. Each team member works on their own
branch and opens pull requests into `main`.

| Contributor | Branch |
| --- | --- |
| Manansh | `Manansh` |
| Aman | `Aman` |
| Aryan | `Aryan` |
| Aditya | `Aditya` |

The repository has five branches: `main` and the four team branches above.
Branch names are case-sensitive.

```sh
git clone https://github.com/MananshAhuja/thread-library.git
cd thread-library
git switch Aman  # Replace with your own branch name.
```

Commit and push changes to your branch, then open a pull request into `main`
when ready to integrate. Each teammate needs collaborator write access to push;
creating a named branch does not grant account access.

## Build

Requires Make, a C11 compiler, and a C++17 compiler.

```sh
make
make check
make clean
```

`make` and `make check` validate the declarations and header compatibility.
They do not build a working library or run functional tests. Add implementation
and linking rules as the library develops. `make examples`, `make tests`, and
`make benchmarks` currently report that these files are empty and exit with
an error instead of reporting a successful build.

Generated files belong under `build/` and are ignored by Git.
`build/.gitkeep` preserves that otherwise-empty directory on GitHub.

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
│   ├── context.h
│   ├── scheduler_internal.h
│   ├── structure.h
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
