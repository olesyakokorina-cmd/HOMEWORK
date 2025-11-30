#pragma once

#include <queue>
#include <thread>
#include <vector>
#include <functional>
#include <mutex>
#include <condition_variable>

class parallel_scheduler {
public:
    parallel_scheduler(size_t capacity);
    ~parallel_scheduler();

    void run(std::function<void()> func);

private:
    void worker_thread();

    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex mtx;
    std::condition_variable cv;

    bool stop;
};

