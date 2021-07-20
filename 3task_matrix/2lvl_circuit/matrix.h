#pragma once

#include "matrixbuf.h"

#include <iostream>

namespace se {
	template <typename T>
	class Matrix final : private detail::MatrixBuf<T> {
		using detail::MatrixBuf<T>::data_;
		using detail::MatrixBuf<T>::size_;
		using detail::MatrixBuf<T>::used_;

		using detail::MatrixBuf<T>::matrix_;
		using detail::MatrixBuf<T>::rows_;
		using detail::MatrixBuf<T>::columns_;

		using detail::MatrixBuf<T>::swap;
	public:
		Matrix(std::size_t rows, std::size_t columns, T value = T{}) :
			detail::MatrixBuf<T>(rows, columns) {
			std::cout << "ctor" << std::endl;
			for (int i = 0; i < rows_; ++i)
				for (int j = 0; j < columns_; ++j) {
					// Forward reference
					detail::construct(&matrix_[i][j], value);
					used_++;
				}
		}

		Matrix(const Matrix& rhs) : 
			detail::MatrixBuf<T>(rhs.rows_, rhs.columns_) {
			std::cout << "Copy ctor" << std::endl;
			for (int i = 0; i < rows_; ++i)
				for (int j = 0; j < columns_; ++j) {
					detail::construct(&matrix_[i][j], rhs.matrix_[i][j]);
					used_++;
				}
		}

		Matrix(Matrix&& rhs) noexcept {
			std::cout << "Move ctor" << std::endl;
			swap(rhs);
		}

		Matrix& operator=(const Matrix& rhs) {
			std::cout << "operator=()" << std::endl;
			Matrix tmp(rhs);
			swap(tmp);
			return *this;
		}

		Matrix& operator=(Matrix&& rhs) {
			std::cout << "move operator=()" << std::endl;
			Matrix tmp(std::move(rhs));
			swap(tmp);
			return *this;
		}
	};
}
