#ifndef UTILS_H
#define UTILS_H

#include <glm/glm.hpp>
#include <iostream>

template <typename matrix_t>
constexpr void PrintMatrix(const matrix_t& m) {
    constexpr size_t size = m.length();
    for (auto i = 0; i < size; ++i) {
        for (auto j = 0; j < size; ++j) {
            std::cout << m[i][j] << '\t';
        }
        std::cout << '\n';
    }
}

template <typename matrix_t>
constexpr void PrintMatrixRowMajor(const matrix_t& m) {
    constexpr size_t size = m.length();
    for (auto i = 0; i < size; ++i) {
        for (auto j = 0; j < size; ++j) {
            std::cout << m[j][i] << '\t';
        }
        std::cout << '\n';
    }
}


#endif // !UTILS_H