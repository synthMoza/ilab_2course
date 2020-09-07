#pragma once

#include <stddef.h>
#include <list>
#include <iostream>
#include <unordered_map>

template<class U, class KeyT = int>
class cacheData {
public:
	static int cur_id;

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
template <class T, class KeyT = int> class ArcHash {
	std::list<T*> T1;
	std::list<T*> T2;
	std::list<T*> B1;
	std::list<T*> B2;

	typedef typename std::list<T*>::iterator ListIt;
	std::unordered_map<KeyT, ListIt> hash;

	size_t p;
	size_t c;
public:
	ArcHash(size_t cache_size);

	bool lookup(const T* elem);

	~ArcHash();
};
