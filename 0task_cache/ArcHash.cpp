#include "ArcHash.h"

int cur_id = 1;

template<class U, class KeyT>
inline cacheData<U, KeyT>::cacheData(U _data) :
		data(_data), id(cur_id++) {
}

template<class U, class KeyT>
inline cacheData<U, KeyT>::~cacheData() {
	// Destroy
}

template<class T, class KeyT = int>
inline ArcHash<T, KeyT>::ArcHash(size_t cache_size) :
		c(cache_size), p(0) {

}

template<class T, class KeyT = int>
inline bool ArcHash<T, KeyT>::lookup(const T *elem) {



	return true;
}

template<class T, class KeyT = int>
inline ArcHash<T, KeyT>::~ArcHash() {
	// Destroy
}
