#pragma once

#include <exception>
#include <functional>
#include <utility>

extern "C" {
#include "thread.h"
#include "scheduler.h"
}

class Thread;
extern "C" void threadCppTrampoline(void* argument);

class Thread {
public:
    Thread() = default;

    Thread(const Thread&) = delete;
    Thread& operator=(const Thread&) = delete;
    Thread(Thread&&) = delete;
    Thread& operator=(Thread&&) = delete;

    template <typename Callable>
    int start(Callable&& callable) {
        if (created_) {
            return -1;
        }

        std::function<void()> task(std::forward<Callable>(callable));
        return startTask(std::move(task));
    }

    int join();
    void rethrowIfFailed() const;

    static int runScheduler();
    static void yield();

private:
    friend void threadCppTrampoline(void* argument);
    void runTask();
    int startTask(std::function<void()> task);

    // Keep Thread objects alive until the scheduler has finished using them.
    thread_control_block controlBlock_{};
    std::function<void()> task_{};
    std::exception_ptr exception_{};
    bool created_ = false;
};
