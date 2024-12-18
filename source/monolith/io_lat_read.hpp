#ifndef IO_LAT_READ_HPP
#define IO_LAT_READ_HPP

#include <string>

void measureIOLatency(const std::string &filePath, size_t blockSize, int repetitions, int num_threads);

#endif 