#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <unistd.h>
#include <sys/wait.h>

void measureIOLatency(const std::string &filePath, size_t blockSize, int repetitions) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filePath << std::endl;
        return;
    }

    // Get file size
    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    if (fileSize < blockSize) {
        std::cerr << "Error: File size is smaller than block size" << std::endl;
        return;
    }

    std::vector<char> buffer(blockSize);
    std::srand(std::time(nullptr));

    double totalLatency = 0.0;

    for (int i = 0; i < repetitions; ++i) {
        size_t randomOffset = (std::rand() % (fileSize / blockSize)) * blockSize;
        file.seekg(randomOffset, std::ios::beg);

        auto start = std::chrono::high_resolution_clock::now();
        file.read(buffer.data(), blockSize);
        auto end = std::chrono::high_resolution_clock::now();

        if (!file) {
            std::cerr << "Error: Could not read from file" << std::endl;
            return;
        }

        std::chrono::duration<double> elapsed = end - start;
        totalLatency += elapsed.count();
    }

    double averageLatency = totalLatency / repetitions;
    std::cout << "Average IO latency for block size " << blockSize << " bytes: " << averageLatency << " seconds" << std::endl;
}

// #include <iostream>
// #include <fstream>
// #include <vector>
// #include <cstdlib>
// #include <ctime>
// #include <chrono>
// #include <unistd.h>
// #include <sys/wait.h>

// void measureIOLatency(const std::string &filePath, size_t blockSize, int repetitions = 10000) {
//     std::ifstream file(filePath, std::ios::binary);
//     if (!file.is_open()) {
//         std::cerr << "Error: Could not open file " << filePath << std::endl;
//         return;
//     }

//     // Get file size
//     file.seekg(0, std::ios::end);
//     size_t fileSize = file.tellg();
//     file.seekg(0, std::ios::beg);

//     if (fileSize < blockSize) {
//         std::cerr << "Error: File size is smaller than block size" << std::endl;
//         return;
//     }

//     std::vector<char> buffer(blockSize);
//     std::srand(std::time(nullptr));

//     double totalLatency = 0.0;

//     for (int i = 0; i < repetitions; ++i) {
//         size_t randomOffset = (std::rand() % (fileSize / blockSize)) * blockSize;
//         file.seekg(randomOffset, std::ios::beg);

//         auto start = std::chrono::high_resolution_clock::now();
//         file.read(buffer.data(), blockSize);
//         auto end = std::chrono::high_resolution_clock::now();

//         if (!file) {
//             std::cerr << "Error: Could not read from file" << std::endl;
//             return;
//         }

//         std::chrono::duration<double> elapsed = end - start;
//         totalLatency += elapsed.count();
//     }

//     double averageLatency = totalLatency / repetitions;
//     std::cout << "Average IO latency for block size " << blockSize << " bytes: " << averageLatency << " seconds" << std::endl;
// }