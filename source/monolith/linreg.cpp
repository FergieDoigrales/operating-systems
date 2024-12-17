#include "linreg.hpp"
#include <iostream>
#include <vector>
#include <pthread.h>
#include <numeric>
#include <cmath>
#include <unistd.h>

// Структура для передачи данных в поток
struct ThreadData {
    const std::vector<double>* x;
    const std::vector<double>* y;
    int repetitions;
    int start;
    int end;
};

// Функция, выполняемая в потоке
// void* calculateSums(void* arg) {
//     ThreadData* data = static_cast<ThreadData*>(arg);
//     double local_sum_x = 0.0;
//     double local_sum_y = 0.0;
//     double local_sum_xy = 0.0;
//     double local_sum_x2 = 0.0;

//     std::cout << "Thread " << pthread_self() << " started." << std::endl;

//     for (int i = data->start; i < data->end; ++i) {
//         local_sum_x += (*data->x)[i];
//         local_sum_y += (*data->y)[i];
//         local_sum_xy += (*data->x)[i] * (*data->y)[i];
//         local_sum_x2 += (*data->x)[i] * (*data->x)[i];
//     }

//     *data->sum_x += local_sum_x;
//     *data->sum_y += local_sum_y;
//     *data->sum_xy += local_sum_xy;
//     *data->sum_x2 += local_sum_x2;

//     std::cout << "Thread " << pthread_self() << " finished its work." << std::endl;

//     // Добавляем задержку, чтобы потоки были видны в htop
//     sleep(1);

//     return nullptr;
// }

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

// Функция для построения модели линейной регрессии
void linearRegression(const std::vector<double>& x, const std::vector<double>& y, int num_threads, int repetitions) {
    int n = x.size();
    std::vector<pthread_t> threads(num_threads);
    std::vector<ThreadData> thread_data(num_threads);

    int chunk_size = n / num_threads;
    int reps_per_thread = repetitions / num_threads;

    for (int i = 0; i < num_threads; ++i) {
        thread_data[i] = {&x, &y, reps_per_thread, i * chunk_size, (i + 1) * chunk_size};
        if (i == num_threads - 1) {
            thread_data[i].end = n; // Последний поток обрабатывает оставшиеся элементы
        }
        pthread_create(&threads[i], nullptr, calculateSums, &thread_data[i]);
    }

    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], nullptr);
    }
}

// #include "linreg.hpp"
// #include <iostream>
// #include <vector>
// #include <pthread.h>
// #include <numeric>
// #include <cmath>

// // Структура для передачи данных в поток
// struct ThreadData {
//     const std::vector<double>* x;
//     const std::vector<double>* y;
//     double* sum_x;
//     double* sum_y;
//     double* sum_xy;
//     double* sum_x2;
//     int start;
//     int end;
//     int repetitions;
// };

// // Функция, выполняемая в потоке
// void* calculateSums(void* arg) {
//     ThreadData* data = static_cast<ThreadData*>(arg);
//     for (int r = 0; r < data->repetitions; ++r) {
//         double local_sum_x = 0.0;
//         double local_sum_y = 0.0;
//         double local_sum_xy = 0.0;
//         double local_sum_x2 = 0.0;

//         for (int i = data->start; i < data->end; ++i) {
//             local_sum_x += (*data->x)[i];
//             local_sum_y += (*data->y)[i];
//             local_sum_xy += (*data->x)[i] * (*data->y)[i];
//             local_sum_x2 += (*data->x)[i] * (*data->x)[i];
//         }

//         *data->sum_x += local_sum_x;
//         *data->sum_y += local_sum_y;
//         *data->sum_xy += local_sum_xy;
//         *data->sum_x2 += local_sum_x2;
//     }

//     return nullptr;
// }

// // Функция для построения модели линейной регрессии
// void linearRegression(const std::vector<double>& x, const std::vector<double>& y, int num_threads, int repetitions) {
//     int n = x.size();
//     double sum_x = 0.0;
//     double sum_y = 0.0;
//     double sum_xy = 0.0;
//     double sum_x2 = 0.0;

//     std::vector<pthread_t> threads(num_threads);
//     std::vector<ThreadData> thread_data(num_threads);

//     int chunk_size = n / num_threads;
//     int reps_per_thread = repetitions / num_threads;

//     for (int i = 0; i < num_threads; ++i) {
//         thread_data[i] = {&x, &y, &sum_x, &sum_y, &sum_xy, &sum_x2, i * chunk_size, (i + 1) * chunk_size, reps_per_thread};
//         if (i == num_threads - 1) {
//             thread_data[i].end = n; // Последний поток обрабатывает оставшиеся элементы
//         }
//         pthread_create(&threads[i], nullptr, calculateSums, &thread_data[i]);
//     }

//     for (int i = 0; i < num_threads; ++i) {
//         pthread_join(threads[i], nullptr);
//     }

//     double b = (n * sum_xy - sum_x * sum_y) / (n * sum_x2 - sum_x * sum_x);
//     double a = (sum_y - b * sum_x) / n;

//     std::cout << "Linear Regression Model: y = " << a << " + " << b << " * x" << std::endl;
// }