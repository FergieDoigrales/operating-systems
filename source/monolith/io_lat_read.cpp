#include "io_lat_read.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <pthread.h>
#include <unistd.h>
#include <random>

struct ThreadData {
    const std::string* filePath;
    size_t blockSize;
    int repetitions;
    double* totalLatency;
};

void* threadFunc(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);
    std::ifstream file(*data->filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << *data->filePath << std::endl;
        return nullptr;
    }

    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    if (fileSize < data->blockSize) {
        std::cerr << "Error: File size is smaller than block size" << std::endl;
        return nullptr;
    }

    std::vector<char> buffer(data->blockSize);
    std::srand(std::time(nullptr));

     // исправленный плюсовый ген рандомных чисел
    std::random_device rd;
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<size_t> dis(0, fileSize / data->blockSize - 1); 

    double localLatency = 0.0;

    for (int i = 0; i < data->repetitions; ++i) {
        size_t randomOffset = dis(gen) * data->blockSize;
        file.seekg(randomOffset, std::ios::beg);

        auto start = std::chrono::high_resolution_clock::now();
        file.read(buffer.data(), data->blockSize);
        auto end = std::chrono::high_resolution_clock::now();

        if (!file) {
            std::cerr << "Error: Could not read from file" << std::endl;
            return nullptr;
        }

        std::chrono::duration<double> elapsed = end - start;
        localLatency += elapsed.count();
    }

    *data->totalLatency += localLatency;
    sleep(1);
    return nullptr;
}

void measureIOLatency(const std::string &filePath, size_t blockSize, int repetitions, int num_threads) {
    std::vector<pthread_t> threads(num_threads);
    std::vector<ThreadData> thread_data(num_threads);
    double totalLatency = 0.0;

    int reps_per_thread = repetitions / num_threads;
    int remaining_reps = repetitions % num_threads;

    for (int i = 0; i < num_threads; ++i) {
        int thread_reps = reps_per_thread + (i < remaining_reps ? 1 : 0);
        thread_data[i] = {&filePath, blockSize, thread_reps, &totalLatency};
        pthread_create(&threads[i], nullptr, threadFunc, &thread_data[i]);
    }

    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], nullptr);
    }

    double averageLatency = totalLatency / repetitions;
    std::cout << "Average IO latency for block size " << blockSize << " bytes: " << averageLatency << " seconds" << std::endl;
}