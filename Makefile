CC = cc
CXX = c++
AR = ar
CPPFLAGS += -Iinclude -Iinternal
CFLAGS += -std=c11 -Wall -Wextra -Wpedantic
CXXFLAGS += -std=c++17 -Wall -Wextra -Wpedantic

C_SOURCES := $(wildcard src/thread/*.c src/scheduler/*.c src/sync/*.c)
CPP_SOURCES := $(wildcard src/cpp/*.cpp)
C_OBJECTS := $(patsubst %.c,build/%.o,$(C_SOURCES))
CPP_OBJECTS := $(patsubst %.cpp,build/%.o,$(CPP_SOURCES))
PROGRAM_C := $(wildcard examples/*.c tests/*.c benchmarks/*.c)
PROGRAM_CPP := $(wildcard examples/*.cpp)
PROGRAMS := $(patsubst %.c,build/%,$(PROGRAM_C)) $(patsubst %.cpp,build/%,$(PROGRAM_CPP))

.PHONY: all examples tests benchmarks clean help
all: build/libthread.a build/libthread_cpp.a

build/libthread.a: $(C_OBJECTS)
	$(AR) rcs $@ $^

build/libthread_cpp.a: $(CPP_OBJECTS)
	$(AR) rcs $@ $^

build/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

build/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

build/%: %.c build/libthread.a
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(LDFLAGS) build/libthread.a $(LDLIBS) -o $@

build/%: %.cpp build/libthread_cpp.a build/libthread.a
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< $(LDFLAGS) build/libthread_cpp.a build/libthread.a $(LDLIBS) -o $@

examples: $(filter build/examples/%,$(PROGRAMS))
tests: $(filter build/tests/%,$(PROGRAMS))
benchmarks: $(filter build/benchmarks/%,$(PROGRAMS))

clean:
	rm -rf build/src build/examples build/tests build/benchmarks
	rm -f build/libthread.a build/libthread_cpp.a

help:
	@echo 'make             Build starter C and C++ static archives'
	@echo 'make examples    Compile example placeholders'
	@echo 'make tests       Compile test placeholders (does not run tests)'
	@echo 'make benchmarks  Compile benchmark placeholders'
	@echo 'make clean       Remove generated build files'

-include $(C_OBJECTS:.o=.d) $(CPP_OBJECTS:.o=.d)
