#include "parallel_scheduler.h"

parallel_scheduler::parallel_scheduler(size_t capacity) : stop(false) {
    for (size_t i = 0; i < capacity; ++i) {
        workers.emplace_back(&parallel_scheduler::worker_thread, this);
    }
}

parallel_scheduler::~parallel_scheduler() {
    {
        std::lock_guard<std::mutex> lock(mtx);
        stop = true;  
    }
    cv.notify_all(); 

    for (auto &t : workers) {
        if (t.joinable())
            t.join();
    }
}

void parallel_scheduler::run(std::function<void()> func) {
    {
        std::lock_guard<std::mutex> lock(mtx);
        tasks.push(func);
    }
    cv.notify_one();
}

void parallel_scheduler::worker_thread() {
    while (true) {
        std::function<void()> task;

        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this] { return stop || !tasks.empty(); });

            if (stop && tasks.empty())
                return; 

            task = tasks.front();
            tasks.pop();
        }

        task();
    }
}

