#include "Thread.hpp"

#include <utility>

int Thread::startTask(std::function<void()> task) {
    if (created_ || !task) {
        return -1;
    }

    task_ = std::move(task);
    const int result = thread_create(&controlBlock_, &threadCppTrampoline, this);
    if (result == 0) {
        created_ = true;
    } else {
        task_ = {};
    }
    return result;
}

int Thread::join() {
    if (!created_) {
        return -1;
    }

    if (current_thread != nullptr) {
        const int result = thread_join(&controlBlock_);
        if (result == 0) {
            rethrowIfFailed();
        }
        return result;
    }

    if (controlBlock_.state != TERMINATED) {
        const int result = scheduler_dispatch();
        if (controlBlock_.state != TERMINATED) {
            return result == 0 ? -1 : result;
        }
    }

    rethrowIfFailed();
    return 0;
}

void Thread::rethrowIfFailed() const {
    if (exception_) {
        std::rethrow_exception(exception_);
    }
}

int Thread::runScheduler() {
    return scheduler_dispatch();
}

void Thread::yield() {
    thread_yield();
}

void Thread::runTask() {
    try {
        task_();
    } catch (...) {
        // Do not let a C++ exception unwind through the C scheduler or assembly.
        exception_ = std::current_exception();
    }
}

extern "C" void threadCppTrampoline(void* argument) {
    static_cast<Thread*>(argument)->runTask();
}
