CC ?= cc
CXX ?= c++
CFLAGS ?= -O2 -g -std=c11 -Wall -Wextra -Iinclude
CXXFLAGS ?= -O2 -g -std=c++17 -Wall -Wextra -Iinclude
LIBOBJ = thread/thread.o thread/thread_create.o thread/scheduler.o thread/mutex.o thread/semaphore.o context/context.o context_switch.o

.PHONY: all test clean
all: thread_demo sync_test switch_benchmark cpp_demo

thread_demo: main.o $(LIBOBJ)
	$(CC) $^ -o $@
sync_test: tests/sync_test.o $(LIBOBJ)
	$(CC) $^ -o $@
switch_benchmark: tests/switch_benchmark.o $(LIBOBJ)
	$(CC) $^ -o $@
cpp_demo: cpp_demo.o thread/thread_cpp.o $(LIBOBJ)
	$(CXX) $^ -o $@
thread/thread_cpp.o: thread/thread.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: %.S
	$(CC) -c $< -o $@
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
test: sync_test switch_benchmark
	./sync_test
	./switch_benchmark
clean:
	rm -f $(LIBOBJ) main.o cpp_demo.o thread/thread_cpp.o tests/*.o thread_demo sync_test switch_benchmark cpp_demo
