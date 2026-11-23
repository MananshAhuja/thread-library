#!/usr/bin/env bash
set -euo pipefail

# Run this on Linux x86-64 from any folder.
cd "$(dirname "$0")"

objects=(
    thread/thread.o thread/thread_create.o thread/scheduler.o
    thread/mutex.o thread/semaphore.o context/context.o context_switch.o
)
make "${objects[@]}" thread/thread_cpp.o
mkdir -p ../build/presentation

"${CC:-cc}" -O2 -std=c11 -Wall -Wextra -Iinclude \
    presentation_demo.c "${objects[@]}" -o ../build/presentation/presentation_c
"${CXX:-c++}" -O2 -std=c++17 -Wall -Wextra -Iinclude \
    presentation_demo.cpp thread/thread_cpp.o "${objects[@]}" \
    -o ../build/presentation/presentation_cpp

echo '=== C demo ==='
../build/presentation/presentation_c
echo '=== C++ demo ==='
../build/presentation/presentation_cpp
