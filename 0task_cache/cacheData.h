#pragma once

//! The template of the class that should be used
//! with this ARC cache realization. Its consists of the element
//! of the type to be cached and its id (automatically sets id of each object)
template<class U, class KeyT = int>
class cacheData {
public:
	U data;
	KeyT id;

	cacheData(U _data);
	cacheData();
	~cacheData();
};

int cur_id = 1;

template<class U, class KeyT>
inline cacheData<U, KeyT>::cacheData(U _data) :
		data(_data), id(cur_id++) {
}

template<class U, class KeyT>
inline cacheData<U, KeyT>::cacheData() {
	data = 0;
	id = 0;
}

template<class U, class KeyT>
inline cacheData<U, KeyT>::~cacheData() {
	// Destroy
}
