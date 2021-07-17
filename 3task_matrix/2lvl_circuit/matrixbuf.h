#pragma once

#include <iostream>

namespace detail {
    /* Helper functions */
    // Create object's copy in the raw memory
    template <typename T, typename U>
    void copy_construct(T* p, const U& value) {
        new (p) T(value);
    }
    // Destroy object without deleting memory
    template <class T>
    void destroy(T* p) {
        p->~T();
    }
    // Destroy objects with iterators
    template <typename FwdIter>
    void destroy(FwdIter first, FwdIter last) {
        while (first != last)
            destroy(&*first++);
    }

    /* Base class for Matrix to work with memory */
    template <typename T>
    class MatrixBuf {
    public:
        // Delete copy, move constructor and assignment operator
        MatrixBuf(const MatrixBuf& rhs) = delete;
        MatrixBuf(MatrixBuf&& rhs) = delete;
        MatrixBuf& operator=(const MatrixBuf& rhs) = delete;
    protected:
        T** data_;
        std::size_t rows_;
        std::size_t columns_;
    
        MatrixBuf(std::size_t rows = 0, std::size_t columns = 0) :
            data_ (nullptr), rows_ (rows), columns_ (columns) {
                if (rows_ != 0 && columns != 0) {
                    // Allocate raw memory for rows
                    data_ = static_cast<T**>(::operator new(sizeof(T*) * rows_));
                    // Allocate raw memory for each row (columns)
                    for (int i = 0; i < rows_; ++i) {
                        data_[i] = static_cast<T*>(::operator new(sizeof(T) * columns_));
                    }
                }
            }
        void swap(MatrixBuf& rhs) noexcept {
            std::swap(data_, rhs.data_);
            std::swap(rows_, rhs.rows_);
            std::swap(columns_, rhs.columns_);
        }
        ~MatrixBuf() noexcept {
            // Clear objects from the matrix
            for (int i = 0; i < rows_; ++i)
                destroy(data_[i], data_[i] + columns_);
            // Clear raw memory
            for (int i = 0; i < rows_; ++i)
                ::operator delete(data_[i]);
            ::operator delete(data_);
        }
    };
}