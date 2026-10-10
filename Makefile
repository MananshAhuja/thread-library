CC = cc
CXX = c++
CPPFLAGS += -Iinclude -Iinternal
CFLAGS += -std=c11 -Wall -Wextra
CXXFLAGS += -std=c++17 -Wall -Wextra

C_SOURCES := $(wildcard src/thread/*.c src/scheduler/*.c src/sync/*.c)
CPP_SOURCES := $(wildcard src/cpp/*.cpp)
HEADERS := $(wildcard include/*.h internal/*.h)
CPP_HEADERS := $(wildcard include/*.hpp)

.PHONY: all check examples tests benchmarks clean help
all: check
	@echo 'Declarations checked. Library implementation is not yet available.'

check:
	$(CC) $(CPPFLAGS) $(CFLAGS) -Werror -fsyntax-only $(C_SOURCES)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -Werror -fsyntax-only $(CPP_SOURCES)
	@set -e; for header in $(HEADERS); do \
		printf '#include "%s"\n' "$$header" | $(CC) $(CPPFLAGS) -I. $(CFLAGS) -Werror -x c -fsyntax-only -; \
		printf '#include "%s"\n' "$$header" | $(CXX) $(CPPFLAGS) -I. $(CXXFLAGS) -Werror -x c++ -fsyntax-only -; \
	done
	@set -e; for header in $(CPP_HEADERS); do \
		printf '#include "%s"\n' "$$header" | $(CXX) $(CPPFLAGS) -I. $(CXXFLAGS) -Werror -x c++ -fsyntax-only -; \
	done

examples tests benchmarks:
	@echo '$@ files are empty; add implementations and build rules before running them.'
	@exit 1

clean:
	rm -rf build/src build/examples build/tests build/benchmarks
	rm -f build/libthread.a build/libthread_cpp.a

help:
	@echo 'make / make check  Check C and C++ declarations; no library is built'
	@echo 'make clean         Remove generated build files'
