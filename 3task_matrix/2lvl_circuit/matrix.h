#pragma once

#include "matrixbuf.h"

namespace se {
    // Matrix class
    template <typename T>
    class Matrix final : private detail::MatrixBuf<T> {
        // Proxy class for the sake of usability [][]
        struct ProxyRow {
            T *row_;
            const T &operator[](int n) const { return row_[n]; }
            T &operator[](int n) { return row_[n]; }
        };
    public:
        using detail::MatrixBuf<T>::data_;
        using detail::MatrixBuf<T>::rows_;
        using detail::MatrixBuf<T>::columns_;

        /* Constructors and assignment operators */

        Matrix(std::size_t rows, std::size_t columns, T value = T{}) : detail::MatrixBuf<T>(rows, columns) {
            // Construct all elements in the memory (as the matrix is always filled with something, it can't be empty)
            for (std::size_t i = 0; i < rows_; ++i)
                for (std::size_t j = 0; j < columns_; ++j)
                    detail::copy_construct(&data_[i][j], value);
        }
        Matrix(const Matrix& rhs) : detail::MatrixBuf<T>(rhs.rows_, rhs.columns_) {
            for (std::size_t i = 0; i < rows_; ++i)
                for (std::size_t j = 0; j < columns_; ++j)
                    detail::copy_construct(&data_[i][j], rhs.data_[i][j]);
        }
        template <typename U>
        Matrix(const Matrix<U>& rhs) : detail::MatrixBuf<T>(rhs.rows_, rhs.columns_) {
            // Useful coercion constructor
            for (std::size_t i = 0; i < rows_; ++i)
                for (std::size_t j = 0; j < columns_; ++j)
                    detail::copy_construct(&data_[i][j], rhs.data_[i][j]);
        }
        Matrix(Matrix&& rhs) : detail::MatrixBuf<T>(rhs.rows_, rhs.columns_) {
            // Move constructor allocates memory and swaps the buffer 
            swap(rhs);
        }
        Matrix& operator=(const Matrix& rhs) {
            Matrix tmp(rhs);
            swap(rhs);
            return *this;
        }
        Matrix& operator=(Matrix&& rhs) {
            Matrix tmp(rhs);
            swap(rhs);
            return *this;
        }
    };
}