#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>

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

int main() {
    std::string filePath = "test_file_2k.bin";
    size_t fileSize = 2048; 

    createTestFile(filePath, fileSize);

    return 0;
}