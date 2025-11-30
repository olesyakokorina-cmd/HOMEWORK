#include "parallel_scheduler.h"
#include <iostream>
#include <unistd.h>

int main() {
    try {
        parallel_scheduler pool(3);

        for (int i = 0; i < 10; ++i) {
	    int task_id = i;	
            pool.run([task_id]() {
                std::cout << "Task " << task_id << " is running" << std::endl;
                usleep(200 * 1000); // 200 мс
            });
        }

        pool.wait_all();

    } catch (const std::invalid_argument& e) {
        std::cerr << "Error creating thread pool: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

