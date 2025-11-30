// demo.cpp
#include "parallel_scheduler.h"
#include <iostream>
#include <chrono>
#include <thread>

int main() {
    parallel_scheduler pool(3);

    for (int i = 0; i < 10; ++i) {
        pool.run([i]() {
            std::cout << "Task " << i 
                      << " is running in thread " << std::this_thread::get_id() 
                      << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        });
    }

    std::this_thread::sleep_for(std::chrono::seconds(3));
}

