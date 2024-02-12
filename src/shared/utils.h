#ifndef UTILS_H
#define UTILS_H

#include <glm/glm.hpp>
#include <iostream>

namespace Utils {
namespace Print {

template <typename matrix_t>
constexpr void Matrix(const matrix_t& m) {
    constexpr size_t size = m.length();
    for (auto i = 0; i < size; ++i) {
        for (auto j = 0; j < size; ++j) {
            std::cout << m[i][j] << '\t';
        }
        std::cout << '\n';
    }
}

template <typename matrix_t>
constexpr void MatrixRowMajor(const matrix_t& m) {
    constexpr size_t size = m.length();
    for (auto i = 0; i < size; ++i) {
        for (auto j = 0; j < size; ++j) {
            std::cout << m[j][i] << '\t';
        }
        std::cout << '\n';
    }
}

template <typename vec_t>
constexpr void Vec(const vec_t& v) {
    constexpr size_t size = v.length();
    if constexpr (size == 4) {
        std::cout << "x: " << v.x << " y: " << v.y << " z: " << v.z << " w: " << v.w << std::endl;
    } else if constexpr (size == 3) {
        std::cout << "x: " << v.x << " y: " << v.y << " z: " << v.z << std::endl;
    }
}
}; // namespace Print
}; // namespace Utils

#endif // !UTILS_H
