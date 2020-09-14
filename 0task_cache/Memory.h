#pragma once

#include <iostream>

template<class T>
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

template<class T>
inline Memory<T>::Memory(int _size) :
		size(_size) {
	data = new T[size];
}

template<class T>
inline void Memory<T>::fill_rand() {
	int cur_id = 1;

	for (int i = 0; i < size; i++) {
		data[i].data = std::rand() % 100 + 1;
		data[i].id = cur_id++;
	}

}

template<class T>
inline void Memory<T>::display() {
	std::cout << "MEMORY\n";
	std::cout << "size=" << size << "\n";

	for (int i = 0; i < size; i++) {
		std::cout << "data[" << i << "]: ";
		std::cout << "DATA=" << data[i].data << ", ";
		std::cout << "ID=" << data[i].id << "\n";
	}
}

template<class T>
inline Memory<T>::~Memory() {
	delete[] data;
}
