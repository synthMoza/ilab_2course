#pragma once

#include <cstdlib>
#include <iostream>

const int IDS_AMOUNT = 1000000;

template<class T, class KeyT = int>
class Memory {
public:
	T *data;
	int size;

	Memory(int _size);

	// Fills the memory (assuming T and KeyT is integer, for tests)
	void fill_rand();

	// Print the whle memory information
	void display();

	~Memory();
};

template<class T, class KeyT>
inline Memory<T, KeyT>::Memory(int _size) :
		size(_size) {
	data = new T[size];
}

template<class T, class KeyT>
inline void Memory<T, KeyT>::fill_rand() {
	int cur_id = 1;

	for (int i = 0; i < size; i++) {
		data[i].data = std::rand() % 100 + 1;
		data[i].id = cur_id++;
	}

}

template<class T, class KeyT>
inline void Memory<T, KeyT>::display() {
	std::cout << "MEMORY\n";
	std::cout << "size=" << size << "\n";

	for (int i = 0; i < size; i++) {
		std::cout << "data[" << i << "]: ";
		std::cout << "DATA=" << data[i].data << ", ";
		std::cout << "ID=" << data[i].id << "\n";
	}
}

template<class T, class KeyT>
inline Memory<T, KeyT>::~Memory() {
	delete[] data;
}
