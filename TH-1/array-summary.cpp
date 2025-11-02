#include <iostream>
#include <vector>
#include <thread>
#include <cstdlib>
#include <ctime>

void sum_part(const std::vector<int>& arr, size_t start, size_t end, long& result) {
    for (size_t i = start; i < end; ++i) {
        result += arr[i];
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: ./array-summary <N> <M>" << std::endl;
        return 1;
    }

    size_t N = atoi(argv[1]); 
    int M = atoi(argv[2]);    

    if (N == 0 || M <= 0) {
        std::cerr << "N must be greater than 0 and M must be positive!" << std::endl;
        return 1;
    }

    std::vector<int> arr(N);
    for (size_t i = 0; i < N; ++i) {
        arr[i] = rand() % 100; 
    }

    clock_t start_time = clock();
    long sum_no_threads = 0;
    for (size_t i = 0; i < N; ++i) {
        sum_no_threads += arr[i];
    }
    clock_t end_time = clock();
    double duration_no_threads = double(end_time - start_time) / CLOCKS_PER_SEC;

    std::vector<std::thread> threads;
    std::vector<long> partial_sums(M, 0); 

    start_time = clock();

    size_t chunk_size = N / M;
    for (int i = 0; i < M; ++i) {
        size_t start = i * chunk_size;
        size_t end = (i == M - 1) ? N : start + chunk_size;
        threads.push_back(std::thread(sum_part, std::cref(arr), start, end, std::ref(partial_sums[i])));
    }

    for (auto& t : threads) {
        t.join();
    }

    long sum_with_threads = 0;
    for (long partial_sum : partial_sums) {
        sum_with_threads += partial_sum;
    }

    end_time = clock();
    double duration_with_threads = double(end_time - start_time) / CLOCKS_PER_SEC;

    std::cout << "Time spent without threads: " << duration_no_threads << " seconds" << std::endl;
    std::cout << "Time spent with M threads: " << duration_with_threads << " seconds" << std::endl;

    return 0;
}


