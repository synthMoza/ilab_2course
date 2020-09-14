#pragma once

#include <list>
#include <unordered_map>

#include "Memory.h"

template<class U, class KeyT = int>
class beladyData {
public:
	U data;
	KeyT id;

	KeyT next_usage;

	beladyData();
};

template<class U, class KeyT>
inline beladyData<U, KeyT>::beladyData() :
		data(0), id(0), next_usage(-1) {
}

template<class T, class KeyT = int>
class beladyCache {
	std::list<T> data;

	typedef typename std::list<T>::iterator ListIt;
	std::unordered_map<KeyT, ListIt> hash_data;

	int cache_size;
	int mem_size;

	int cur_elem;

	Memory<beladyData<int, KeyT>> *access_order;
public:
	beladyCache(int c_size, Memory<beladyData<int, KeyT>> *mem,
			int access_times);

	bool lookup(const T *elem);
	void printList();
	void printMem();
};

template<class T, class KeyT>
inline beladyCache<T, KeyT>::beladyCache(int c_size,
		Memory<beladyData<int, KeyT>> *mem, int access_times) :
		cache_size(c_size), cur_elem(0), mem_size(access_times), access_order(
				mem) {
	// Predict the next usage of all elements (-1 if wasn't used)
	for (int i = 0; i < mem_size - 1; i++) {
		auto elem_id = access_order->data[i].id;

		for (int j = i + 1; j < mem_size; j++) {
			// Found the element
			if (elem_id == access_order->data[j].id) {
				access_order->data[i].next_usage = j;
				break;
			}
		}
	}
}

template<class T, class KeyT>
inline bool beladyCache<T, KeyT>::lookup(const T *elem) {
	assert(elem);

	auto hit = hash_data.find(elem->id);

	if (hit == hash_data.end()) {
		// Element is not in the cache

		if (data.size() < cache_size) {
			data.push_front(*elem);
			hash_data[elem->id] = data.begin();

			return false;
		}

		int father_usage = -1;
		ListIt father_it = data.end();

		for (auto i = data.begin(); i != data.end(); i = std::next(i)) {
			if (i->next_usage == -1) {
				// Never gonna
				father_usage = 0;
				father_it = i;
				break;
			}

			if (i->next_usage > father_usage) {
				father_usage = i->next_usage;
				father_it = i;
			}
		}

		// std::cout << "Father - " << father_usage << "\n";

		if (father_usage == -1) {
			// None of the elements will be used
			father_it = data.begin();
		}

		// Insert the element
		ListIt new_it = data.insert(father_it, *elem);
		hash_data[elem->id] = new_it;

		// Delete the previous element from the cache
		hash_data.erase(father_it->id);
		data.erase(father_it);

		return false;
	} else {
		// It is in the cache
		int next_ = hit->second->next_usage;

		hit->second->next_usage = access_order->data[next_].next_usage;

		return true;
	}
}

template<class T, class KeyT>
inline void beladyCache<T, KeyT>::printList() {
	std::cout << "Belady cache:\n";
	for (auto elem : data) {
		std::cout << elem.data << "\n";
	}
	std::cout << "\n";
	std::cout << "===============\n";
}

template<class T, class KeyT>
inline void beladyCache<T, KeyT>::printMem() {
	std::cout << "Belady cache:\n";

	for (int i = 0; i < mem_size; i++) {
		std::cout << "Elem: " << access_order->data[i].data << "\n";
		std::cout << "Next usage: " << access_order->data[i].next_usage << "\n";
	}
	std::cout << "\n";
	std::cout << "===============\n";
}
