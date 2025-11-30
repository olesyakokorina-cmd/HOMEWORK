#pragma once

#include <queue>
#include <vector>
#include <functional>
#include <pthread.h>
#include <stdexcept>

class parallel_scheduler {
public:
    static const size_t MAX_CAPACITY = 64;	

    parallel_scheduler(size_t capacity);
    ~parallel_scheduler();

    void run(std::function<void()> func);
    void wait_all();

private:
    static void* thread_entry(void* arg);
    void worker_thread();

    std::vector<pthread_t> workers;
    std::queue<std::function<void()>> tasks;

    pthread_mutex_t mtx;
    pthread_cond_t cv;
    pthread_cond_t cv_finished;

    bool stop;
    size_t capacity;
};

