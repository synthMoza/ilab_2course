#pragma once

#include <cstdlib>
#include <utility>

namespace detail {
	template <typename T, typename U> 
	void construct(T* p, const U& rhs) {
		new (p) T(rhs);
	}

	template <class T> 
	void destroy(T* p) {
		p->~T();
	}

	template <typename FwdIter>
	void destroy(FwdIter first, FwdIter last) {
		while (first++ != last)
			destroy(&*first);
	}

	template <typename T>
	class Buf {
	protected:
		using size_type = std::size_t;

		T* data_;
		size_type used_;
		size_type size_;

		Buf() : size_ (0), used_ (0), data_ (nullptr) {}
		Buf(size_type size) : size_(size), used_ (0),
			data_(static_cast<T*>(::operator new(sizeof(T) * size))) {}

		void swap(Buf& rhs) noexcept {
			std::swap(data_, rhs.data_);
			std::swap(size_, rhs.size_);
			std::swap(used_, rhs.used_);
		}

		~Buf() {
			destroy(data_, data_ + used_);
			if (data_ != nullptr)
				::operator delete(data_);
		}
	public:
		Buf(const Buf& rhs) = delete;
		Buf& operator=(const Buf& rhs) = delete;
	};
}
