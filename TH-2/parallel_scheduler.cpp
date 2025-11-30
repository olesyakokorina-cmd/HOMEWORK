#include "parallel_scheduler.h"
#include <unistd.h>

parallel_scheduler::parallel_scheduler(size_t capacity_) : stop(false), capacity(capacity_) {
    if(capacity_ == 0 || capacity_ > MAX_CAPACITY) {
    	throw std::invalid_argument("Capacity must be > 0 and <= MAX_CPACITY");
    }

    pthread_mutex_init(&mtx, nullptr);
    pthread_cond_init(&cv, nullptr);
    pthread_cond_init(&cv_finished, nullptr);

    workers.resize(capacity);    
    for (size_t i = 0; i < capacity; ++i) {
        pthread_create(&workers[i], nullptr, &parallel_scheduler::thread_entry, this);
    }
}

parallel_scheduler::~parallel_scheduler() {
    pthread_mutex_lock(&mtx);
    stop = true;
    pthread_cond_broadcast(&cv);
    pthread_mutex_unlock(&mtx);

    for(size_t i = 0; i < capacity; ++i) {
    	pthread_join(workers[i], nullptr);
    }

    pthread_mutex_destroy(&mtx);
    pthread_cond_destroy(&cv);
    pthread_cond_destroy(&cv_finished);
}

void parallel_scheduler::run(std::function<void()> func) {
    pthread_mutex_lock(&mtx);
    tasks.push(func);
    pthread_cond_signal(&cv);
    pthread_mutex_unlock(&mtx);
    
}

void parallel_scheduler::wait_all() {
    pthread_mutex_lock(&mtx);
    while (!tasks.empty()) {
        pthread_cond_wait(&cv_finished, &mtx);
    }
    pthread_mutex_unlock(&mtx);
}

void* parallel_scheduler::thread_entry(void* arg) {
    parallel_scheduler* scheduler = static_cast<parallel_scheduler*>(arg);
    scheduler->worker_thread();
    return nullptr;
}

void parallel_scheduler::worker_thread() {
    while (true) {
        pthread_mutex_lock(&mtx);
        while (!stop && tasks.empty()) {
            pthread_cond_wait(&cv, &mtx);
        }

        if (stop && tasks.empty()) {
            pthread_mutex_unlock(&mtx);
            break;
        }

        auto task = tasks.front();
        tasks.pop();
        pthread_mutex_unlock(&mtx);

        task(); 

        pthread_mutex_lock(&mtx);
        if (tasks.empty()) {
            pthread_cond_signal(&cv_finished);
        }
        pthread_mutex_unlock(&mtx);
    }
}

