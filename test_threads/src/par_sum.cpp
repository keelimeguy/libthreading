#include "par_sum.h"

#include <vector>
#include <memory>
#include <iostream>

#include "Core.h"

struct WorkerArg {
    int *arr;
    int size;
    int sum;

    WorkerArg(int *arr, int size, int sum)
        : arr(arr), size(size), sum(sum) {}
};

THREAD_FUNC(worker_task, int,WorkerArg) {
    for (int i = 0; i < arg->size; ++i)
        arg->sum += arg->arr[i];
    THREAD_RETURN(&(arg->sum));
}

long par_sum(int *arr, int size, int num_threads) {
    int delta = size / num_threads;
    int rem = size % num_threads;

    if (delta == 0) num_threads = size;

    long sum = 0;

    std::vector<std::shared_ptr<Thread<int,WorkerArg>>> workers(num_threads);

    for (int i = 0; i < num_threads; ++i) {
        int part_size = delta + (int)(i < rem);
        int start = i*(delta + (int)(i <= rem)) + (int)(i > rem)*rem;

        if (start >= size) break;

        workers[i] = Core::MakeThread<int,WorkerArg>(-1, worker_task, (&arr[0]) + start, part_size, 0);
    }

    int running = num_threads;
    while (running) {
        for (int i = 0; i < num_threads; ++i) {
            if (workers[i]->ResultPending()) {
                sum += (long)(*(workers[i]->Join()));
                --running;
            }
        }
    }

    return sum;
}
