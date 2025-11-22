#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <ctime>

struct ThreadData {
    int* arr;
    int start;
    int end;
    long long result;
};

void* thread_sum(void* arg)
{
    ThreadData* data = (ThreadData*)arg;
    long long sum = 0;

    for (int i = data->start; i < data->end; i++)
        sum += data->arr[i];

    data->result = sum;
    return NULL;
}

int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <N> <M>\n";
        return 1;
    }

    int N = atoi(argv[1]);
    int M = atoi(argv[2]);

    if (N <= 1000000) {
        std::cout << "N must be > 1,000,000\n";
        return 1;
    }
    if (M <= 0) {
        std::cout << "M must be >= 1\n";
        return 1;
    }

    int* arr = new int[N];

    srand(time(NULL));
    for (int i = 0; i < N; i++)
        arr[i] = rand() % 100 + 1;

    clock_t t1 = clock();

    long long sum_single = 0;
    for (int i = 0; i < N; i++)
        sum_single += arr[i];

    clock_t t2 = clock();
    double time_single = double(t2 - t1) / CLOCKS_PER_SEC;

    pthread_t* threads = new pthread_t[M];
    ThreadData* tdata = new ThreadData[M];

    int chunk = N / M;
    int remainder = N % M;

    clock_t t3 = clock();

    for (int i = 0; i < M; i++) {
        tdata[i].arr = arr;
        tdata[i].start = i * chunk;
        tdata[i].end = (i == M - 1) ? (i + 1) * chunk + remainder
                                    : (i + 1) * chunk;
        pthread_create(&threads[i], NULL, thread_sum, &tdata[i]);
    }

    for (int i = 0; i < M; i++)
        pthread_join(threads[i], NULL);

    long long sum_multi = 0;
    for (int i = 0; i < M; i++)
        sum_multi += tdata[i].result;

    clock_t t4 = clock();
    double time_multi = double(t4 - t3) / CLOCKS_PER_SEC;

    std::cout << "Time spent without threads: " << time_single << " seconds\n";
    std::cout << "Time spent with " << M << " threads: " << time_multi << " seconds\n";

    delete[] threads;
    delete[] tdata;
    delete[] arr;

    return 0;
}

