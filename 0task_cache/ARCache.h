#pragma once

#include <stddef.h>
#include <list>
#include <iostream>
#include <unordered_map>

//! The template of the class that should be used
//! with this ARC cache realization. Its consists of the element
//! of the type to be cached and its id (automatically sets id of each object)
template<class U, class KeyT = int>
class cacheData {
public:
	U data;
	KeyT id;

	cacheData(U _data);
	~cacheData();
};

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
public:
	ARCache(size_t cache_size);

	//! Print all the lists that ARC uses (for debug only)
	void printLists();
	//! Change the sizes of all lists according to new value of 'p'
	//! This function is only called in 'lookup' method
	void setLists();
	//! Looks if the given element is in the cache and doing ARC algorithm
	bool lookup(const T *elem);

	//! Delete some element from certain list (for T_i - delete element and put it into B_i)
	void deleteFromT1(const T *elem);
	void deleteFromT2(const T *elem);
	void deleteFromB1(const T *elem);
	void deleteFromB2(const T *elem);

	bool isOK();

	~ARCache();
};

int cur_id = 1;

template<class U, class KeyT>
inline cacheData<U, KeyT>::cacheData(U _data) :
		data(_data), id(cur_id++) {
}

template<class U, class KeyT>
inline cacheData<U, KeyT>::~cacheData() {
	// Destroy
}

template<class T, class KeyT>
inline ARCache<T, KeyT>::ARCache(size_t cache_size) :
		c(cache_size), p(0) {
}

template<class T, class KeyT>
inline bool ARCache<T, KeyT>::lookup(const T *elem) {
	auto hit = hash_T2.find(elem->id);

	// If we didn't find the element in T2
	if (hit == hash_T2.end()) {
		hit = hash_T1.find(elem->id);

		// If we didn't find the element in T1 either
		if (hit == hash_T1.end()) {
			hit = hash_B1.find(elem->id);

			// If we didn't find the element in B1
			if (hit == hash_B1.end()) {
				hit = hash_B2.find(elem->id);

				if (hit == hash_B2.end()) {
					// Didn't find it anywhere

					// If the T1 list is full
					if (T1.size() == c) {
						deleteFromT1(&(*T1.end()));
					}

					T1.push_front(*elem);
					hash_T1[elem->id] = T1.begin();

					p++;

					setLists();
					return false;
				} else {
					// Found it in B2
					float _temp = B1.size() / B2.size();
					_temp = std::abs(_temp);

					p = std::max(0, p - std::max(static_cast<int>(_temp), 1));

					setLists();
					return false;
				}
			} else {
				// Found it in B1
				float _temp = B1.size() / B2.size();
				_temp = std::abs(_temp);

				p = std::min(c, p + std::max(1, static_cast<int>(_temp)));

				setLists();
				return false;
			}
		} else {
			// We found the element in T1, should move it to the top of T2
			if (T2.size() == c) {
				deleteFromT2(&(*T2.end()));
			}

			T2.push_front(*hash_T1[elem->id]);
			hash_T2[elem->id] = hash_T1[elem->id];

			T1.erase(hash_T1[elem->id]);
			hash_T1.erase(elem->id);

			p--;

			setLists();
			return true;
		}
	} else {
		// We found the element in T2
		T2.erase(hash_T2[elem->id]);
		T2.erase(hash_T2[elem->id]);
		T2.push_front(*hash_T2[elem->id]);

		setLists();
		return true;
	}

	std::cerr << "Error! " << __PRETTY_FUNCTION__ << "\n";
	std::cerr << "Reached the end of the function unexpectedly!\n";
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

			int _boundary = T2.size() - (c - p);
			for (int i = 0; i < _boundary; i++) {
				deleteFromT2(&(*T2.end()));
			}
		}
	} else if (p < T1.size()) {
		int _boundary = T1.size() - p;
		for (int i = 0; i < _boundary; i++) {
			deleteFromT2(&(*T1.end()));
		}
	}

	// If 'p' didn't change, do nothing
}

template<class T, class KeyT>
inline void ARCache<T, KeyT>::printLists() {
	std::cout << "================\n";

	std::cout << "T1:\n";
	for (auto elem : T1) {
		std::cout << elem.data << "\n";
	}

	std::cout << "T2:\n";
	for (auto elem : T2) {
		std::cout << elem.data << "\n";
	}

	std::cout << "B1:\n";
	for (auto elem : B1) {
		std::cout << elem.data << "\n";
	}

	std::cout << "B2:\n";
	for (auto elem : B2) {
		std::cout << elem.data << "\n";
	}

	std::cout << "c = " << c << "\n";
	std::cout << "p = " << p << "\n";

	std::cout << "================\n";
}

template<class T, class KeyT>
inline void ARCache<T, KeyT>::deleteFromT1(const T *elem) {
	T1.erase(hash_T1[elem->id]);
	hash_T1.erase(elem->id);

	// If B1 list is full
	if (B1.size() == c) {
		deleteFromB1(&(*B1.end()));
	}

	B1.push_front(*elem);
	hash_B1[elem->id] = B1.begin();

	p--;
}

template<class T, class KeyT>
inline void ARCache<T, KeyT>::deleteFromT2(const T *elem) {
	T2.erase(hash_T2[elem->id]);
	hash_T2.erase(elem->id);

	// If B2 list is full
	if (B2.size() == c) {
		deleteFromB2(&(*B2.end()));
	}

	B2.push_front(*elem);
	hash_B2[elem->id] = B2.begin();

	p++;
}

template<class T, class KeyT>
inline void ARCache<T, KeyT>::deleteFromB1(const T *elem) {
	B1.erase(hash_B1[elem->id]);
	hash_B1.erase(elem->id);
}

template<class T, class KeyT>
inline void ARCache<T, KeyT>::deleteFromB2(const T *elem) {
	B2.erase(hash_B2[elem->id]);
	hash_B2.erase(elem->id);
}

template<class T, class KeyT>
inline bool ARCache<T, KeyT>::isOK() {
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
inline ARCache<T, KeyT>::~ARCache() {
	// Destroy
}
