#pragma once

#include <stdexcept>
#include <cmath>
#include <initializer_list>
		
#include "matrixbuf.h"

#include <iostream>

namespace se {
	template <typename T>
	class Matrix final : private detail::MatrixBuf<T> {
		// The accuracy
		const double eps = 1e-06;

		using detail::MatrixBuf<T>::data_;
		using detail::MatrixBuf<T>::size_;
		using detail::MatrixBuf<T>::used_;

		using detail::MatrixBuf<T>::matrix_;
		using detail::MatrixBuf<T>::rows_;
		using detail::MatrixBuf<T>::columns_;

		using detail::MatrixBuf<T>::swap;

		// Proxy object to allow "[][]" acces to the memory
		struct ProxyRow {
            T *row;
            const T &operator[](int n) const { return row[n]; }
            T &operator[](int n) { return row[n]; }
        };
	public:
		// Access matrix size type
        using size_type = typename detail::MatrixBuf<T>::size_type;

		// Standart constructor
		// @param rows Number of rows in the matrix
		// @param columns Number of columns in the matrix
		// @param value Value to fill the matrix, default value is taken by default
		Matrix(size_type rows, size_type columns, T value = T{}) :
			detail::MatrixBuf<T>(rows, columns) {
			for (int i = 0; i < rows_; ++i)
				for (int j = 0; j < columns_; ++j) {
					// Forward reference
					detail::construct(&matrix_[i][j], value);
					used_++;
				}
		}
		// Construct the matrix out of initializer lists
		// The size of outer list determines the "row" size
		// The size of each inner list determines the "column" size 
		// If they are not the same, throws exception
		Matrix(std::initializer_list<std::initializer_list<T>> matrix) {
			if (matrix.size() == 0)
				throw std::runtime_error("Error! Matrix can't have 0 rows");

			Matrix<T> tmp(matrix.size(), matrix.begin()->size());
			*this = std::move(tmp);

			typename std::initializer_list<T>::size_type sz = matrix.begin()->size();
			if (sz == 0)
				throw std::runtime_error("Error! Row can't be 0 size");

			size_type i = 0;
			for (auto&& row : matrix) {
				if (sz != row.size())
					throw std::runtime_error("Error! Rows must have the same size");
				size_type j = 0;
				for (auto&& value : row) {
					// Forward reference
					detail::construct(&matrix_[i][j], value);
					used_++;
					j++;
				}

				++i;
			}
		}

		// Copy constructor
		Matrix(const Matrix& rhs) : 
			detail::MatrixBuf<T>(rhs.rows_, rhs.columns_) {
			for (int i = 0; i < rows_; ++i)
				for (int j = 0; j < columns_; ++j) {
					detail::construct(&matrix_[i][j], rhs.matrix_[i][j]);
					used_++;
				}
		}

		// Coercion copy constructor
		template <typename U>
		Matrix(const Matrix<U>& rhs) :
			detail::MatrixBuf<T>(rhs.getRows(), rhs.getColumns()) {
			for (int i = 0; i < rows_; ++i)
				for (int j = 0; j < columns_; ++j) {
					detail::construct(&matrix_[i][j], rhs[i][j]);
					used_++;
				}
		}

		Matrix(Matrix&& rhs) noexcept {
			swap(rhs);
		}

		Matrix& operator=(const Matrix& rhs) {
			Matrix tmp(rhs);
			swap(tmp);
			return *this;
		}

		Matrix& operator=(Matrix&& rhs) {
			Matrix tmp(std::move(rhs));
			swap(tmp);
			return *this;
		}

		// Access matrix through "[][]"
		ProxyRow operator[](size_type row) {
			if (row >= rows_)
				throw std::out_of_range("Index is out of range!");
			
			return ProxyRow{matrix_[row]};
		}

		ProxyRow operator[](size_type row) const {
			if (row >= rows_)
				throw std::out_of_range("Index is out of range!");
			
			return ProxyRow{matrix_[row]};
		}

		// Getters
		size_type getColumns() const {
			return columns_;
		}

		size_type getRows() const {
			return rows_;
		}

		// Get the eye matrix
        static Matrix eye(int n) {
			Matrix tmp(n, n);

			for (int i = 0; i < n; ++i)
				tmp.matrix_[i][i] = 1;

			return tmp;
		}
        // Get zero matrix m * n
        static Matrix zero(int n, int m) {
			return Matrix(n, m);
		}
        // Get the rotation matrix of size n and between axis of angle
        static Matrix<float> rotation(int n, int i, int j, float angle) {
			Matrix<float> res = Matrix<float>::eye(n);

			if (i == j || i < 0 || j < 0)
				throw std::logic_error("Invalid rotation indices!");

			// Ensure that i < j
			if (i > j) {
				int tmp = i;
				i = j;
				j = tmp;
			}

			res[i][i] = std::cos(angle);
			res[i][j] = -std::sin(angle);
			res[j][i] = std::sin(angle);
			res[j][j] = std::cos(angle);

			return res;
		}
		// Negate the matrix
        Matrix& negate() & {
			size_type length = rows_ * columns_;
			for (int i = 0; i < length; ++i)
				data_[i] *= -1;
			
			return *this;
		}
        // Transpose the matrix
        Matrix& transpose() & {
			Matrix<T> tmp(getColumns(), getRows());

			for (int i = 0; i < rows_; ++i)
				for (int j = 0; j < columns_; ++j)
					tmp.matrix_[j][i] = matrix_[i][j];

			swap(tmp);
			return *this;
		}
		// Get the negative matrix (unary minus)
		Matrix operator-() const {
			Matrix<T> tmp(*this);
			tmp.negate();
			return tmp;
		}

		/* Chained operators */
		
		Matrix &operator+=(const Matrix& rhs) & {
			if (rows_ != rhs.rows_ || columns_ != rhs.columns_)
				throw std::runtime_error("Matrices must have the same size");

			size_type length = rows_ * columns_;
			for (int i = 0; i < length; ++i)
				data_[i] += rhs.data_[i];

			return *this;
		}

        Matrix &operator-=(const Matrix& rhs) & {
			if (rows_ != rhs.rows_ || columns_ != rhs.columns_)
				throw std::runtime_error("Matrices must have the same size");

			size_type length = rows_ * columns_;
			for (int i = 0; i < length; ++i)
				data_[i] -= rhs.data_[i];

			return *this;
		}

		// Multiply by scalar
        Matrix& operator*=(T value) & {
			size_type length = rows_ * columns_;
			for (int i = 0; i < length; ++i)
				data_[i] *= value;

			return *this;
		}
		// Raise to the nth power (not negative)
        Matrix& operator^=(unsigned int n) & {
			Matrix<T> mul{*this};
			for (int i = 1; i < n; ++i)
				(*this) *= mul;

			return *this;
		}

		Matrix<T> operator*(const Matrix<T>& rhs) {
			Matrix<T> tmp{*this};
			tmp *= rhs; 
			return tmp;
		}
        Matrix& operator*=(const Matrix &rhs) & {
			if (columns_ != rows_)
				throw std::logic_error("Multiplication \"A*B\" is defined only for matrices with A.columns==B.rows");

			Matrix<T> res(rows_, rhs.columns_);

			for (int i = 0; i < rows_; ++i) {
				T* r_res = &res.matrix_[i][0];
				for (int k = 0; k < columns_; ++k) {
					const T* r_rhs = &rhs.matrix_[k][0];
					T r_lhs = matrix_[i][k];

					for (int j = 0; j < rhs.columns_; ++j) {
						r_res[j] += r_lhs * r_rhs[j];
					}
				}
			}

			*this = res;
			return *this;
		}

		// Remove the given row (indices begin with 0)
		Matrix& removeRow(size_type row) {
			if (row >= rows_)
				throw std::out_of_range("Row is out of range!");
			
			Matrix<T> tmp(rows_ - 1, columns_);
			for (size_type i = 0; i < row; ++i)
				for (int j = 0; j < columns_; ++j)
					tmp.matrix_[i][j] = matrix_[i][j];
			for (size_type i = row + 1; i < rows_; ++i)
				for (int j = 0; j < columns_; ++j)
					tmp.matrix_[i - 1][j] = matrix_[i][j];
			
			*this = std::move(tmp);
			return *this;
		}

        Matrix& removeColumn(int column) & {
			if (column >= columns_)
				throw std::out_of_range("Column is out of range!");
			
			Matrix<T> tmp(rows_, columns_ - 1);
			for (size_type j = 0; j < column; ++j)
				for (int i = 0; i < rows_; ++i)
					tmp.matrix_[i][j] = matrix_[i][j];
			for (size_type j = column + 1; j < columns_; ++j)
				for (int i = 0; i < rows_; ++i)
					tmp.matrix_[i][j - 1] = matrix_[i][j];
			
			*this = std::move(tmp);
			return *this;
		}

		Matrix& swapRows(int i, int j) & {
			if (i >= rows_ || j >= rows_)
				throw std::out_of_range("Indices are out of range!");
			
			std::swap(matrix_[i], matrix_[j]);
			return *this;
		}

		Matrix& subRows(size_type i, size_type j, T j_k) & {
			if (i >= rows_ || j >= columns_)
				throw std::out_of_range("Indices are out of range!");

			for (int m = 0; m < columns_; ++m) {
				matrix_[i][m] -= (j_k * matrix_[j][m]);
				if (std::abs(matrix_[i][m]) < eps)
					matrix_[i][m] = 0;
			}

			return *this;
		}

		// Find the maximum value in the given column
		// @return Row of this element
    	int columnMax(int j) const {
			int max = j;

			for (int i = j; i < rows_; ++i)
				if (std::abs(matrix_[i][j]) > std::abs(matrix_[max][j]))
					max = i;

			return max;
		}

		// Get the determinant of this matrix A
    	T determinant() const {
			if (columns_ != rows_)
				throw std::logic_error("Only square matrices have determinant");

			// Gauss elimination
			Matrix<long double> temp{*this};

			int sign = 1;
			long double res = 1;
			int max = 0;
			long double max_val = 0;

			for (int i = 0; i < rows_ - 1; ++i) {
				// Find the maximum value in the current column
				// and put the max to the top (if neccesary)
				max = temp.columnMax(i);
				max_val = temp[max][i];
				if (max != i) {
					temp.swapRows(max, i);
					sign *= -1;
				}
				
				if (max_val == 0)
					return 0;

				// Zero all elements under this maximum value
				for (int j = i + 1; j < rows_; ++j) {
					temp.subRows(j, i, temp[j][i] / max_val);
				}

				res *= temp[i][i];
			}

			res *= (sign * temp[rows_ - 1][rows_ - 1]);
			return static_cast<T>(res);
		}

		// Get the matrix of cofactors
    	Matrix cofactors() const {
			Matrix<T> res{*this};

			for (int i = 0; i < rows_; ++i)
				for (int j = 0; j < columns_; ++j) {
					matrix_[i][j] = getMinor(i, j);

					if ((i + j) % 2 != 0) {
						res[i][j] *= -1;
					}
				}

			return res;
		}

		// Get the minor M(i, j)
    	T getMinor(size_t i, size_t j) const {
			if (i >= rows_ || j >= columns_)
				throw std::out_of_range("Indices are out of range!");
	
			Matrix<T> tmp{*this};
			tmp.removeRow(i);
			tmp.removeColumn(j);

			return tmp.determinant();
		}

		// Inverse the matrix
		Matrix& inverse() & {
			float det = determinant();

			*this = cofactors();
			transpose();
			*this *= (1 / det);

			return *this;
		}
	};

	/* Matrix operators */

	template <typename T>
	Matrix<T> operator^(const Matrix<T> &lhs, int n) {
		Matrix<T> tmp{lhs};
		tmp ^= n;
		return tmp;
	}

	template <typename T>
	Matrix<T> operator+(const Matrix<T>& lhs, const Matrix<T>& rhs) {
		Matrix<T> tmp{lhs};
		tmp += rhs;
		return tmp;
	}

	template <typename T>
	Matrix<T> operator-(const Matrix<T>& lhs, const Matrix<T>& rhs) {
		Matrix<T> tmp{lhs};
		tmp -= rhs;
		return tmp;
	}

	template <typename T>
	Matrix<T> operator*(const Matrix<T>& mtx, T value) {
		Matrix<T> tmp{mtx};
		tmp *= value; 
		return tmp;
	}

	/* Logic operators */

	template <typename T>
    bool operator==(const Matrix<T> &lhs, const Matrix<T> &rhs) {
		typename Matrix<T>::size_type columns = lhs.getColumns();
		typename Matrix<T>::size_type rows = lhs.getRows();

		if (columns != rhs.getColumns() || rows != rhs.getRows())
			return false;
		
		for (typename Matrix<T>::size_type i = 0; i < rows; ++i)
			for (typename Matrix<T>::size_type j = 0; j < columns; ++j)
				if (lhs[i][j] != rhs[i][j])
					return false;
		
		return true;
	}

    template <typename T>
    bool operator!=(const Matrix<T> &lhs, const Matrix<T> &rhs) {
		return !(lhs == rhs);
	}

	/* Stream operators */

	template <typename T>
    std::ostream& operator<<(std::ostream &os, const Matrix<T> &rhs) {
		for (typename Matrix<T>::size_type i = 0; i < rhs.getRows(); ++i) {
			for (typename Matrix<T>::size_type j = 0; j < rhs.getColumns(); ++j)
				os << rhs[i][j] << "\t";
			
			os << std::endl;
		}

		return os;
	}

    template <typename T>
    std::istream &operator>>(std::istream &is, const Matrix<T> &rhs) {
		for (typename Matrix<T>::size_type i = 0; i < rhs.getRows(); ++i)
			for (typename Matrix<T>::size_type j = 0; j < rhs.getColumns(); ++j)
				is >> rhs[i][j];

		return is;
	}
}
