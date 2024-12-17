#include "linreg.hpp"
#include <iostream>
#include <vector>
#include <pthread.h>
#include <numeric>
#include <cmath>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/time.h>

struct ThreadData {
    const std::vector<double>* x;
    const std::vector<double>* y;
    int repetitions;
    int start;
    int end;
};


void* calculateSums(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);

    for (int r = 0; r < data->repetitions; ++r) {
        double sum_x = 0.0;
        double sum_y = 0.0;
        double sum_xy = 0.0;
        double sum_x2 = 0.0;

        for (int i = data->start; i < data->end; ++i) {
            sum_x += (*data->x)[i];
            sum_y += (*data->y)[i];
            sum_xy += (*data->x)[i] * (*data->y)[i];
            sum_x2 += (*data->x)[i] * (*data->x)[i];
        }

        double b = (data->end * sum_xy - sum_x * sum_y) / (data->end * sum_x2 - sum_x * sum_x);
        double a = (sum_y - b * sum_x) / data->end;

        std::cout << "Thread " << pthread_self() << " Repetition " << r + 1 << ": Linear Regression Model: y = " << a << " + " << b << " * x" << std::endl;

    }
    sleep(1);

    return nullptr;
}

void linearRegression(const std::vector<double>& x, const std::vector<double>& y, int num_threads, int repetitions) {
    int n = x.size();
    std::vector<pthread_t> threads(num_threads);
    std::vector<ThreadData> thread_data(num_threads);

    int chunk_size = n / num_threads;
    int reps_per_thread = repetitions / num_threads;

    for (int i = 0; i < num_threads; ++i) {
        thread_data[i] = {&x, &y, reps_per_thread, i * chunk_size, (i + 1) * chunk_size};
        if (i == num_threads - 1) {
            thread_data[i].end = n; 
        }
        pthread_create(&threads[i], nullptr, calculateSums, &thread_data[i]);
    }

    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], nullptr);
    }
}
