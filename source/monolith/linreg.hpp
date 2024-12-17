#ifndef LINREG_HPP
#define LINREG_HPP

#include <vector>

// Функция для построения модели линейной регрессии
void linearRegression(const std::vector<double>& x, const std::vector<double>& y, int num_threads, int repetitions);

#endif // LINREG_HPP