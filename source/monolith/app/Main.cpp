// #include <iostream>

// #include "monolith/HelloWorld.hpp"
// #include "monolith/Shell.hpp"

// namespace monolith::app {

// namespace {

// void Main() {
//   std::cout << HelloWorld() << '\n';
//   fergieShell(); 
// }

// }  // namespace

// }  // namespace monolith::app

// int main() {
//   monolith::app::Main();
// }





// #include <iostream>
// #include <string>
// #include "monolith/HelloWorld.hpp"
// #include "monolith/Shell.hpp"
// #include "monolith/io_lat_read.hpp"

// namespace monolith::app {

// void runShell() {
//   std::cout << HelloWorld() << '\n';
//   fergieShell();
// }

// void runIOLatencyTest(const std::string &filePath, int repetitions) {
//   size_t blockSize = 2048; // 2K block size
//   measureIOLatency(filePath, blockSize, repetitions);
// }

// }  // namespace monolith::app

// int main(int argc, char *argv[]) {
//   if (argc < 2) {
//     std::cerr << "Usage: " << argv[0] << " <mode> [args...]" << std::endl;
//     std::cerr << "Modes: shell, io_lat_read" << std::endl;
//     return 1;
//   }

//   std::string mode = argv[1];
//   if (mode == "shell") {
//     monolith::app::runShell();
//   } else if (mode == "io_lat_read") {
//     if (argc != 4) {
//       std::cerr << "Usage: " << argv[0] << " io_lat_read <file_path> <repetitions>" << std::endl;
//       return 1;
//     }
//     std::string filePath = argv[2];
//     int repetitions = std::stoi(argv[3]);
//     monolith::app::runIOLatencyTest(filePath, repetitions);
//   } else {
//     std::cerr << "Unknown mode: " << mode << std::endl;
//     return 1;
//   }

//   return 0;
// }





#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "monolith/HelloWorld.hpp"
#include "monolith/Shell.hpp"
#include "monolith/io_lat_read.hpp"
#include "monolith/linreg.hpp"

// Функция для создания тестового файла
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
  std::cout << HelloWorld() << '\n';
  fergieShell();
}

void runIOLatencyTest(const std::string &filePath, int repetitions) {
  size_t blockSize = 2048; 
  measureIOLatency(filePath, blockSize, repetitions);
}

void runLinreg(int num_threads, int repetitions) {
  // Пример данных
  int dataSize = 1000000; // 1 миллион точек данных
  std::vector<double> x(dataSize);
  std::vector<double> y(dataSize);

  // Заполнение данных случайными значениями
  std::srand(std::time(nullptr));
  for (int i = 0; i < dataSize; ++i) {
    x[i] = std::rand() % 1000;
    y[i] = 2 * x[i] + (std::rand() % 100); // y = 2x + шум
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
    if (argc != 4) {
      std::cerr << "Usage: " << argv[0] << " io_lat_read <file_path> <repetitions>" << std::endl;
      return 1;
    }
    std::string filePath = argv[2];
    int repetitions = std::stoi(argv[3]);
    monolith::app::runIOLatencyTest(filePath, repetitions);
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

// #include <iostream>
// #include <string>
// #include <fstream>
// #include <vector>
// #include <cstdlib>
// #include <ctime>
// #include "monolith/HelloWorld.hpp"
// #include "monolith/Shell.hpp"
// #include "monolith/io_lat_read.hpp"
// #include "monolith/linreg.hpp" // Включаем заголовочный файл для линейной регрессии

// // Функция для создания тестового файла
// void createTestFile(const std::string &filePath, size_t fileSize) {
//     std::ofstream file(filePath, std::ios::binary);
//     if (!file.is_open()) {
//         std::cerr << "Error: Could not create file " << filePath << std::endl;
//         return;
//     }

//     std::vector<char> buffer(fileSize);
//     std::srand(std::time(nullptr));
//     std::generate(buffer.begin(), buffer.end(), std::rand);

//     file.write(buffer.data(), fileSize);
//     file.close();

//     std::cout << "Test file created: " << filePath << " with size " << fileSize << " bytes" << std::endl;
// }

// namespace monolith::app {

// void runShell() {
//   std::cout << HelloWorld() << '\n';
//   fergieShell();
// }

// void runIOLatencyTest(const std::string &filePath, int repetitions) {
//   size_t blockSize = 2048; 
//   measureIOLatency(filePath, blockSize, repetitions);
// }

// void runLinreg(int num_threads, int repetitions) {
//   // Пример данных
//   std::vector<double> x = {1, 2, 3, 4, 5};
//   std::vector<double> y = {2, 4, 5, 4, 5};

//   linearRegression(x, y, num_threads, repetitions);
// }

// }  // namespace monolith::app

// int main(int argc, char *argv[]) {
//   if (argc < 2) {
//     std::cerr << "Usage: " << argv[0] << " <mode> [args...]" << std::endl;
//     std::cerr << "Modes: shell, io_lat_read, create_test_file, linreg" << std::endl;
//     return 1;
//   }

//   std::string mode = argv[1];
//   if (mode == "shell") {
//     monolith::app::runShell();
//   } else if (mode == "io_lat_read") {
//     if (argc != 4) {
//       std::cerr << "Usage: " << argv[0] << " io_lat_read <file_path> <repetitions>" << std::endl;
//       return 1;
//     }
//     std::string filePath = argv[2];
//     int repetitions = std::stoi(argv[3]);
//     monolith::app::runIOLatencyTest(filePath, repetitions);
//   } else if (mode == "create_test_file") {
//     if (argc != 4) {
//       std::cerr << "Usage: " << argv[0] << " create_test_file <file_path> <file_size>" << std::endl;
//       return 1;
//     }
//     std::string filePath = argv[2];
//     size_t fileSize = std::stoull(argv[3]);
//     createTestFile(filePath, fileSize);
//   } else if (mode == "linreg") {
//     if (argc != 4) {
//       std::cerr << "Usage: " << argv[0] << " linreg <num_threads> <repetitions>" << std::endl;
//       return 1;
//     }
//     int num_threads = std::stoi(argv[2]);
//     int repetitions = std::stoi(argv[3]);
//     monolith::app::runLinreg(num_threads, repetitions);
//   } else {
//     std::cerr << "Unknown mode: " << mode << std::endl;
//     return 1;
//   }

//   return 0;
// }
