#pragma once

#include <iomanip>
#include <iostream>

#include "ARCache.h"
#include "cacheData.h"
#include "Memory.h"

//! @brief Test with random data accessed from memory
//!	@param cache_size The size of the cache
//! @param memory_size The size of the memory
//! @param access_times The amount of memory accesses
void unit_test_1(int cache_size, int memory_size, int access_times);

//! @brief Test with median with given range, times that median changes and how many
//! @brief times it jumps across this median
//!	@param cache_size The size of the cache
//! @param memory_size The size of the memory
//! @param range Memory will be accessed at this range across the median
//! @param median_changes How many times median will change
//! @param times How many times it will jump across this median
void unit_test_2(int cache_size, int memory_size, int range, int median_changes,
		int times);

//! @brief Test with probability - range is generated
//! @brief with some probability
//!	@param cache_size The size of the cache
//! @param memory_size The size of the memory
//! @param access_times The amount of memory accesses
void unit_test_3(int cache_size, int memory_size, int access_times);

//! @brief Test cache with input data
//! @param type variable needed only for the type of keys for the cache
template<class KeyT>
void input_test(KeyT type) {
	// For output
	int hit_count = 0;
	float percent = 0;

	int cache_size = 0;
	int memory_size = 0;

	std::cin >> cache_size;
	std::cin >> memory_size;

	ARCache<cacheData<int, KeyT>, KeyT> arc_cache(cache_size);
	Memory<cacheData<int, KeyT>, KeyT> memory(memory_size);

	for (int i = 0; i < memory_size; i++) {
		KeyT key = 0;
		std::cin >> key;
		memory.data[i].id = key;
		memory.data[i].data = key;
		if (arc_cache.lookup(&memory.data[i]))
			hit_count++;
	}

	percent = ((float) hit_count) * 100.f / memory_size;
	std::cout << "Input data test: hits - " << hit_count
			<< ", total amount of requests - " << memory_size << " ("
			<< std::setprecision(3) << percent << "%)" << "\n";
}
