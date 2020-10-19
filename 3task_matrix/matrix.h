#pragma once

#include <iostream>

namespace mofn
{
    template <typename T>
    class Matrix
    {
        // The size of the matrix
        int nrows_;
        int ncolumns_;
        // The array of pointers
        T **data_;

    public:
        //! Constructor of the 'nrows * ncolumns' matrix filled with the value
        Matrix(int nrows, int ncolumns, T value = T{});

        //! Print the matrix
        void print() const;

        //! Get the eye matrix
        static Matrix eye(int n);
        //! Negate the matrix
        Matrix &negate() &;
        //! Get the negative matrix
        Matrix operator-();

        //! Big three
        Matrix(const Matrix &rhs);
        Matrix &operator=(const Matrix &rhs);
        ~Matrix();
    };

    template <typename T>
    Matrix<T>::Matrix(int nrows, int ncolumns, T value) : 
        data_(new T *[nrows]),
        nrows_(nrows),
        ncolumns_(ncolumns)
    {
        for (int i = 0; i < nrows_; ++i)
        {
            data_[i] = new T[ncolumns];
            for (int j = 0; j < ncolumns_; ++j)
            {
                data_[i][j] = value;
            }
        }
    }

    template <typename T>
    Matrix<T>::Matrix(const Matrix &rhs) : 
        data_(new T *[nrows_]),
        nrows_(rhs.nrows_), 
        ncolumns_(rhs.ncolumns_)
    {
        for (int i = 0; i < nrows_; ++i)
        {
            data_[i] = new T[ncolumns_];
            for (int j = 0; j < ncolumns_; ++j)
            {
                data_[i][j] = rhs.data_[i][j];
            }
        }
    }

    template <typename T>
    Matrix<T> &Matrix<T>::operator=(const Matrix &rhs)
    {
        // If we have 'a = a', may return quickly
        if (&rhs == this)
            return *this;

        // If the size of the other matrix is the same, don't reinitilize
        if (nrows_ != rhs.nrows_ || ncolumns_ != rhs.ncolumns_)
        {
            this->~Matrix();

            nrows_ = rhs.nrows_;
            ncolumns_ = rhs.ncolumns_;
            data_ = new T *[nrows_];
        }

        // Copy all values from the right matrix
        for (int i = 0; i < nrows_; ++i)
        {
            data_[i] = new T[ncolumns_];
            for (int j = 0; j < ncolumns_; ++j)
                data_[i][j] = rhs.data_[i][j];
        }

        return *this;
    }

    template <typename T>
    Matrix<T>::~Matrix()
    {
        for (int i = 0; i < nrows_; ++i)
            delete[] data_[i];

        delete[] data_;
    }

    template <typename T>
    void Matrix<T>::print() const
    {
        for (int i = 0; i < nrows_; ++i)
        {
            for (int j = 0; j < ncolumns_; ++j)
            {
                std::cout << data_[i][j] << " ";
            }

            std::cout << std::endl;
        }
    }

    template <typename T>
    Matrix<T> Matrix<T>::eye(int n)
    {
        Matrix res{n, n, 0};

        for (int i = 0; i < n; ++i)
        {
            res.data_[i][i] = 1;
        }

        return res;
    }
    template <typename T>
    Matrix<T> &Matrix<T>::negate() &
    {
        for (int i = 0; i < nrows_; ++i)
            for (int j = 0; j < ncolumns_; ++j)
                data_[i][j] *= -1;

        return *this;
    }

    template <typename T>
    Matrix<T> Matrix<T>::operator-() {
        Matrix<T> tmp = *this;
        return tmp.negate();
    }
}