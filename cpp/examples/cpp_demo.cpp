#include "Mutex.hpp"
#include "Thread.hpp"

#include <exception>
#include <iostream>
#include <list>

int main() {
    int threadCount = 0;
    int incrementsPerThread = 0;

    std::cout << "Enter thread count and increments per thread: ";
    if (!(std::cin >> threadCount >> incrementsPerThread) ||
        threadCount <= 0 || incrementsPerThread <= 0) {
        std::cerr << "Please enter two positive whole numbers.\n";
        return 1;
    }

    Mutex counterMutex;
    long long counter = 0;
    std::list<Thread> threads;

    for (int index = 0; index < threadCount; ++index) {
        threads.emplace_back();
        Thread& thread = threads.back();
        const int threadId = index + 1;

        const int result = thread.start([&counter, &counterMutex, threadId,
                                         incrementsPerThread]() {
            for (int step = 0; step < incrementsPerThread; ++step) {
                LockGuard lock(counterMutex);
                ++counter;
                std::cout << "Thread " << threadId
                          << " incremented counter to " << counter << '\n';

                // Yield while holding the lock so another thread must wait.
                Thread::yield();
            }
        });

        if (result != 0) {
            std::cerr << "Could not create thread " << threadId << ".\n";
            return 1;
        }
    }

    try {
        // From main(), join() runs the cooperative scheduler until this
        // thread has finished. The scheduler also runs the other ready tasks.
        if (threads.front().join() != 0) {
            std::cerr << "The scheduler could not finish the thread tasks.\n";
            return 1;
        }

        for (const Thread& thread : threads) {
            thread.rethrowIfFailed();
        }
    } catch (const std::exception& error) {
        std::cerr << "Thread task failed: " << error.what() << '\n';
        return 1;
    } catch (...) {
        std::cerr << "Thread task failed with an unknown exception.\n";
        return 1;
    }

    const long long expected =
        static_cast<long long>(threadCount) * incrementsPerThread;
    std::cout << "Final counter: " << counter << " (expected " << expected
              << ")\n";
    return counter == expected ? 0 : 1;
}