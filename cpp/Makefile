CC := gcc
CXX := g++
COMPILER_TAG := $(shell $(CC) -dumpfullversion)-$(shell $(CXX) -dumpfullversion)
BUILD_DIR := build/$(COMPILER_TAG)

CFLAGS := -std=c11 -Wall -Wextra -I../include
CXXFLAGS := -std=c++17 -Wall -Wextra -I../include -Iinclude

# context_init.cpp supplies the C-linkage context_init function for this build.
C_SOURCES := \
	../thread/thread_create.c \
	../thread/thread.c \
	../thread/scheduler.c \
	../thread/mutex.c \
	../thread/semaphore.c

CPP_SOURCES := \
	src/context_init.cpp \
	src/Thread.cpp \
	src/Mutex.cpp \
	src/Semaphore.cpp \
	examples/cpp_demo.cpp

C_OBJECTS := $(patsubst ../%.c,$(BUILD_DIR)/c/%.o,$(C_SOURCES))
CPP_OBJECTS := $(patsubst %.cpp,$(BUILD_DIR)/cpp/%.o,$(CPP_SOURCES))
ASM_SOURCE := $(firstword $(wildcard ../context/context_switch.S ../context_switch.S))
ASM_OBJECT := $(BUILD_DIR)/c/context_switch.o
PROGRAM := $(BUILD_DIR)/cpp_demo

.PHONY: all run clean

all: $(PROGRAM)

$(PROGRAM): $(C_OBJECTS) $(CPP_OBJECTS) $(ASM_OBJECT)
	$(CXX) $^ -o $@

$(BUILD_DIR)/c/%.o: ../%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/cpp/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(ASM_OBJECT): $(ASM_SOURCE)
	mkdir -p $(dir $@)
	$(CC) -c $< -o $@

run: $(PROGRAM)
	$(PROGRAM)

clean:
	rm -rf build
