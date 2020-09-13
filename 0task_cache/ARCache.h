#pragma once

#include <cassert>
#include <iostream>
#include <list>
#include <unordered_map>

//! @brief The main class that performs ARC cache algorithm
//! @param T - the type of the currently caching data
//! @param T1  - the LRU list for recent cache entries
//! @param T2 - the LRU list for frequent cache entries (referred at least 2 times)
//! @param B1 - the list of "ghost" entries that are no longer in the cash and
//! have been removed from T1
//! @param B2 - the list of "ghost" entries that are no longer in the cash and
//! have been removed from T2
template<class T, class KeyT = int> class ARCache {
	std::list<T> T1;
	std::list<T> T2;
	std::list<T> B1;
	std::list<T> B2;

	typedef typename std::list<T>::iterator ListIt;
	std::unordered_map<KeyT, ListIt> hash_T1;
	std::unordered_map<KeyT, ListIt> hash_T2;
	std::unordered_map<KeyT, ListIt> hash_B1;
	std::unordered_map<KeyT, ListIt> hash_B2;

	int c;
	int p;

	//! Change the sizes of all lists according to new value of 'p'
	//! This function is only called in 'lookup' method
	void setLists();
	//! Delete some element from certain list (for T_i - delete element and put it into B_i)
	void deleteFromT1(const T *elem);
	void deleteFromT2(const T *elem);
	void deleteFromB1(const T *elem);
	void deleteFromB2(const T *elem);

	void foundNowhere(const T *elem);
	void foundT1(const T *elem);
	void foundT2(const T *elem);
	void foundB1();
	void foundB2();

	bool isOK();
public:
	ARCache(size_t cache_size);

	//! Print all the lists that ARC uses (for debug only)
	void printLists();
	//! Looks if the given element is in the cache and doing ARC algorithm
	bool lookup(const T *elem);
};

template<class T, class KeyT>
inline ARCache<T, KeyT>::ARCache(size_t cache_size) :
		c(cache_size), p(0) {
}

template<class T, class KeyT>
inline bool ARCache<T, KeyT>::lookup(const T *elem) {
	assert(elem);

	if (!isOK()) {
		std::cerr << "Error! " << __PRETTY_FUNCTION__ << "\n";
		std::cerr << "Wrong size of the list!\n";
		exit(-1);
	}

	auto hit = hash_T2.find(elem->id);

	if (hit != hash_T2.end()) {
		foundT2(elem);
		return true;
	}

	hit = hash_T1.find(elem->id);

	if (hit != hash_T1.end()) {
		foundT1(elem);
		return true;
	}

	hit = hash_B1.find(elem->id);

	if (hit != hash_B1.end()) {
		foundB1();
		return false;
	}

	hit = hash_B2.find(elem->id);

	if (hit != hash_T1.end()) {
		foundB2();
		return false;
	}

	foundNowhere(elem);

	return false;
}

template<class T, class KeyT>
inline void ARCache<T, KeyT>::setLists() {
	if (p > T1.size()) {
		// We increased the size of the T1 list
		// The size of T2 must be reduced, and all
		// deleted elements will be put into B2

		if (T2.size() > (c - p)) {
			// If there are elements to be thrown away

			int boundary_ = p - T1.size();
			boundary_ = std::min(boundary_, (int) B1.size());

			for (int i = 0; i < boundary_; i++) {
				ListIt iterator_ = std::prev(T2.end());

				T2.erase(iterator_);
				hash_T2.erase(iterator_->id);

				// If B2 list is full
				if (B2.size() == c) {
					ListIt iterator_ = std::prev(B2.end());

					deleteFromB2(&(*iterator_));
				}

				B2.push_front(*iterator_);
				hash_B2[iterator_->id] = B2.begin();
			}

			for (int i = 0; i < boundary_; i++) {
				ListIt iterator_ = B1.begin();

				// Add to T1
				T1.push_front(*iterator_);
				hash_T1[iterator_->id] = T1.begin();

				// Delete from B1
				deleteFromB1(&(*iterator_));
			}
		}
	} else if (p < T1.size()) {
		int boundary_ = -p + T1.size();
		boundary_ = std::min(boundary_, (int) B2.size());

		for (int i = 0; i < boundary_; i++) {
			ListIt iterator_ = std::prev(T1.end());

			T1.erase(iterator_);
			hash_T1.erase(iterator_->id);

			// If B1 list is full
			if (B1.size() == c) {
				ListIt iterator_ = std::prev(B1.end());

				deleteFromB1(&(*iterator_));
			}

			B1.push_front(*iterator_);
			hash_B1[iterator_->id] = B1.begin();
		}

		for (int i = 0; i < boundary_; i++) {
			ListIt iterator_ = B2.begin();

			// Add to T2
			T2.push_front(*iterator_);
			hash_T2[iterator_->id] = T2.begin();

			// Delete from B2
			deleteFromB2(&(*iterator_));
		}
	}

	// If 'p' didn't change, do nothing
}

template<class T, class KeyT>
inline void ARCache<T, KeyT>::printLists() {
	std::cout << "================\n";

	std::cout << "T1: ";
	for (auto elem : T1) {
		std::cout << elem.data << " ";
	}

	std::cout << "\n";

	std::cout << "T2: ";
	for (auto elem : T2) {
		std::cout << elem.data << " ";
	}

	std::cout << "\n";

	std::cout << "B1: ";
	for (auto elem : B1) {
		std::cout << elem.data << " ";
	}

	std::cout << "\n";

	std::cout << "B2: ";
	for (auto elem : B2) {
		std::cout << elem.data << " ";
	}

	std::cout << "\n";

	std::cout << "c = " << c << "\n";
	std::cout << "p = " << p << "\n";
	std::cout << "T1.size() = " << T1.size() << "\n";
	std::cout << "T2.size() = " << T2.size() << "\n";
	std::cout << "B1.size() = " << B1.size() << "\n";
	std::cout << "B2.size() = " << B2.size() << "\n";

	std::cout << "================\n";
}

template<class T, class KeyT>
inline void ARCache<T, KeyT>::deleteFromT1(const T *elem) {
	assert(elem);

	auto pair_ = hash_T1.find(elem->id);

	if (pair_ == hash_T1.end()) {
		std::cerr << "Error! " << __PRETTY_FUNCTION__ << "\n";
		std::cerr << "Can't find the element " << elem << " in T1!\n";

		exit(-1);
	}

	hash_T1.erase(elem->id);
	T1.erase(pair_->second);

	// If B1 list is full
	if (B1.size() == c) {
		ListIt iterator_ = std::prev(B1.end());

		deleteFromB1(&(*iterator_));
	}

	B1.push_front(*elem);
	hash_B1[elem->id] = B1.begin();

	p--;
}

template<class T, class KeyT>
inline void ARCache<T, KeyT>::deleteFromT2(const T *elem) {
	assert(elem);

	auto pair_ = hash_T2.find(elem->id);

	if (pair_ == hash_T2.end()) {
		std::cerr << "Error! " << __PRETTY_FUNCTION__ << "\n";
		std::cerr << "Can't find the element " << elem << " in T2!\n";

		exit(-1);
	}

	hash_T2.erase(elem->id);
	T2.erase(pair_->second);

	// If B2 list is full
	if (B2.size() == c) {
		ListIt iterator_ = std::prev(B2.end());

		deleteFromB2(&(*iterator_));
	}

	B2.push_front(*elem);
	hash_B2[elem->id] = B2.begin();

	p++;
}

template<class T, class KeyT>
inline void ARCache<T, KeyT>::deleteFromB1(const T *elem) {
	assert(elem);

	ListIt iterator_ = hash_B1.find(elem->id)->second;

	hash_B1.erase(elem->id);
	B1.erase(iterator_);
}

template<class T, class KeyT>
inline void ARCache<T, KeyT>::deleteFromB2(const T *elem) {
	assert(elem);

	ListIt iterator_ = hash_B2.find(elem->id)->second;

	hash_B2.erase(elem->id);
	B2.erase(iterator_);
}

template<class T, class KeyT>
inline bool ARCache<T, KeyT>::isOK() {
	if (p < 0) {
		std::cerr << "Error! " << __PRETTY_FUNCTION__ << "\n";
		std::cerr << "p is negative \'c\'\n";

		return false;
	}

	if (c < 0) {
		std::cerr << "Error! " << __PRETTY_FUNCTION__ << "\n";
		std::cerr << "c is negative \'c\'\n";

		return false;
	}

	if (T1.size() > c) {
		std::cerr << "Error! " << __PRETTY_FUNCTION__ << "\n";
		std::cerr << "The size of T1 is greater than \'c\'\n";

		return false;
	}

	if (T2.size() > c) {
		std::cerr << "Error! " << __PRETTY_FUNCTION__ << "\n";
		std::cerr << "The size of T2 is greater than \'c\'\n";

		return false;
	}

	if (T2.size() + T1.size() > c) {
		std::cerr << "Error! " << __PRETTY_FUNCTION__ << "\n";
		std::cerr << "The size of T2 + T1 is greater than \'c\'\n";

		return false;
	}

	if (B1.size() > c) {
		std::cerr << "Error! " << __PRETTY_FUNCTION__ << "\n";
		std::cerr << "The size of B1 is greater than \'c\'\n";

		return false;
	}

	if (B2.size() > c) {
		std::cerr << "Error! " << __PRETTY_FUNCTION__ << "\n";
		std::cerr << "The size of B2 is greater than \'c\'\n";

		return false;
	}

	return true;
}

template<class T, class KeyT>
inline void ARCache<T, KeyT>::foundNowhere(const T *elem) {
	assert(elem);

	// Didn't find it anywhere

	// If the lists are full
	if (T1.size() + T2.size() == c) {
		ListIt iterator_;

		if (T1.size() != 0) {
			iterator_ = std::prev(T1.end());
			deleteFromT1(&(*iterator_));
		} else {
			iterator_ = std::prev(T2.end());
			deleteFromT2(&(*iterator_));
		}

	}

	T1.push_front(*elem);
	hash_T1[elem->id] = T1.begin();

	p++;

	setLists();
}

template<class T, class KeyT>
inline void ARCache<T, KeyT>::foundT1(const T *elem) {
	assert(elem);

	// We found the element in T1, should move it to the top of T2
	auto pair_ = hash_T1.find(elem->id);

	if (pair_ == hash_T1.end()) {
		std::cerr << "Error! " << __PRETTY_FUNCTION__ << "\n";
		std::cerr << "Can't find the element " << elem << " in T1!\n";

		exit(-1);
	}

	T2.push_front(*elem);
	hash_T2[elem->id] = T2.begin();

	T1.erase(pair_->second);
	hash_T1.erase(elem->id);

	p--;

	setLists();
}

template<class T, class KeyT>
inline void ARCache<T, KeyT>::foundT2(const T *elem) {
	assert(elem);

	auto pair_ = hash_T2.find(elem->id);

	if (pair_ == hash_T2.end()) {
		std::cerr << "Error! " << __PRETTY_FUNCTION__ << "\n";
		std::cerr << "Can't find the element " << elem << " in T2!\n";

		exit(-1);
	}

	T2.push_front(*elem);

	T2.erase(pair_->second);
	hash_T2[elem->id] = T2.begin();

	setLists();
}

template<class T, class KeyT>
inline void ARCache<T, KeyT>::foundB1() {
	// Found it in B1
	assert(B1.size() != 0);

	float temp_ = B2.size() / B1.size();
	temp_ = std::abs(temp_);

	p = std::min(c, p + std::max(1, static_cast<int>(temp_)));

	setLists();
}

template<class T, class KeyT>
inline void ARCache<T, KeyT>::foundB2() {
	// Found it in B2
	assert(B2.size() != 0);

	float temp_ = B1.size() / B2.size();
	temp_ = std::abs(temp_);

	p = std::max(0, p - std::max(static_cast<int>(temp_), 1));

	setLists();
}
