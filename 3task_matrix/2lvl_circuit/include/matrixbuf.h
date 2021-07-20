#pragma once

#include "buf.h"

namespace detail {

	template <typename T>
	class MatrixBuf : public Buf<T> {
	protected:
		using Buf<T>::data_;

		using size_type = typename Buf<T>::size_type;

		T** matrix_;
		size_type rows_;
		size_type columns_;

		MatrixBuf() : matrix_(nullptr), rows_(0), columns_(0) {}
		MatrixBuf(size_type rows, size_type columns) : Buf<T>(rows * columns),
			matrix_ (new T*[rows]), rows_(rows), columns_(columns) {
			for (int i = 0; i < rows_; ++i) {
				matrix_[i] = &data_[i * columns_];
			}
		}
		void swap(MatrixBuf& rhs) noexcept {
			Buf<T>::swap(rhs);
			std::swap(matrix_, rhs.matrix_);
			std::swap(rows_, rhs.rows_);
			std::swap(columns_, rhs.columns_);
		}
		~MatrixBuf() {
			if (matrix_ != nullptr)
				delete[] matrix_;
		}
	public:
		MatrixBuf(const MatrixBuf& rhs) = delete;
		MatrixBuf& operator=(const MatrixBuf& rhs) = delete;
	};
}