#ifndef UTILS_H
#define UTILS_H

#include <glm/glm.hpp>
#include <iostream>

namespace Utils {
namespace Print {

template <typename array_t>
void Array(const array_t& arr) {
    const size_t size = arr.size();
    if (size % 3 != 0) {
        for (auto i = 0; i < size; ++i) {
            std::cout << arr[i] << " ";
        }
    } else {
        for (auto i = 0; i < size / 3; ++i) {
            std::cout << i << ") " <<
                arr[i * 3] << " " <<
                arr[i * 3 + 1] << " " <<
                arr[i * 3 + 2] << " " << std::endl;
        }
    }
    std::cout << '\n';
}

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
