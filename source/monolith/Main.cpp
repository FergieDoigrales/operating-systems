#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <ctime>
#include "Shell.hpp"
#include "io_lat_read.hpp"
#include "linreg.hpp"


void createTestFile(const std::string &filePath, size_t fileSize) {
    std::ofstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Could not create file " << filePath << std::endl;
        return;
    }

    std::vector<char> buffer(fileSize);
    std::srand(std::time(nullptr));
    std::generate(buffer.begin(), buffer.end(), std::rand);

    file.write(buffer.data(), fileSize);
    file.close();

    std::cout << "Test file created: " << filePath << " with size " << fileSize << " bytes" << std::endl;
}

namespace monolith::app {

void runShell() {
  fergieShell();
}

void runIOLatencyTest(const std::string &filePath, int repetitions, int num_threads) {
  size_t blockSize = 2048; 
  measureIOLatency(filePath, blockSize, repetitions, num_threads);
}

void runLinreg(int num_threads, int repetitions) {

  int dataSize = 1000000; 
  std::vector<double> x(dataSize);
  std::vector<double> y(dataSize);

  // Заполнение данных случайными значениями
  std::srand(std::time(nullptr));
  for (int i = 0; i < dataSize; ++i) {
    x[i] = std::rand() % 1000;
    y[i] = 2 * x[i] + (std::rand() % 100); 
  }

  linearRegression(x, y, num_threads, repetitions);
}

}  // namespace monolith::app

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <mode> [args...]" << std::endl;
    std::cerr << "Modes: shell, io_lat_read, create_test_file, linreg" << std::endl;
    return 1;
  }

  std::string mode = argv[1];
  if (mode == "shell") {
    monolith::app::runShell();
  } else if (mode == "io_lat_read") {
    if (argc != 5) {
      std::cerr << "Usage: " << argv[0] << " io_lat_read <file_path> <repetitions> <num_threads>" << std::endl;
      return 1;
    }
    std::string filePath = argv[2];
    int repetitions = std::stoi(argv[3]);
    int num_threads = std::stoi(argv[4]);
    monolith::app::runIOLatencyTest(filePath, repetitions, num_threads);
  } else if (mode == "create_test_file") {
    if (argc != 4) {
      std::cerr << "Usage: " << argv[0] << " create_test_file <file_path> <file_size>" << std::endl;
      return 1;
    }
    std::string filePath = argv[2];
    size_t fileSize = std::stoull(argv[3]);
    createTestFile(filePath, fileSize);
  } else if (mode == "linreg") {
    if (argc != 4) {
      std::cerr << "Usage: " << argv[0] << " linreg <num_threads> <repetitions>" << std::endl;
      return 1;
    }
    int num_threads = std::stoi(argv[2]);
    int repetitions = std::stoi(argv[3]);
    monolith::app::runLinreg(num_threads, repetitions);
  } else {
    std::cerr << "Unknown mode: " << mode << std::endl;
    return 1;
  }

  

  return 0;
}

