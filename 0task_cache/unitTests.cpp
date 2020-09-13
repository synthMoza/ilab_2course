#include "unitTests.h"

#include <iostream>
#include <iomanip>

#include "ARCache.h"
#include "cacheData.h"
#include "Memory.h"

void unit_test_1(int cache_size, int memory_size, int access_times) {
	// For output
	int hit_count = 0;
	float percent = 0;

	ARCache<cacheData<int>> arc_cache(cache_size);
	Memory<cacheData<int>> memory(memory_size);

	// Fill the memory randomly
	memory.fill_rand();

	// Access cache 'access_times' times
	for (int i = 0; i < access_times; i++) {
		bool _res = false;
		int _index = std::rand() % memory_size + 1;

		std::cout << "The data is " << memory.data[_index].data << ".\n";

		_res = arc_cache.lookup(&memory.data[_index]);
		if (_res)
			hit_count++;
	}

	percent = ((float) hit_count) * 100.f / access_times;
	std::cout << "Unit Test 1 (Random): hits - " << hit_count
			<< ", total amount of requests - " << access_times << " ("
			<< std::setprecision(3) << percent << "%)" << "\n";
}

void unit_test_2(int cache_size, int memory_size, int range, int median_changes,
		int times) {
	// For output
	int hit_count = 0;
	float percent = 0;
	int access_times = 0;

	// For median generating
	int median = 0;
	int index = 0;

	ARCache<cacheData<int>> arc_cache(cache_size);
	Memory<cacheData<int>> memory(memory_size);

	// Fill the memory randomly
	memory.fill_rand();

	for (int i = 0; i < median_changes; i++) {
		// Generate new median
		median = std::rand() % (memory_size - 2 * range) + range;

		// std::cout << "Median is " << median << ".\n";

		for (int j = 0; j < times; j++) {
			index = std::rand() % (2 * range) - range;

			// std::cout << "The data is " << memory.data[median + index].data << ".\n";

			if (arc_cache.lookup(&memory.data[median + index]))
				hit_count++;

			// arc_cache.printLists();
		}
	}

	access_times = median_changes * times;
	percent = ((float) hit_count) * 100.f / access_times;
	std::cout << "Unit Test 2 (Median): hits - " << hit_count
			<< ", total amount of requests - " << access_times << " ("
			<< std::setprecision(3) << percent << "%)" << "\n";
}

void unit_test_3(int cache_size, int memory_size, int access_times) {
	// For output
	int hit_count = 0;
	float percent = 0;

	// For median generating
	int probability = 0;
	int index = 0;

	ARCache<cacheData<int>> arc_cache(cache_size);
	Memory<cacheData<int>> memory(memory_size);

	// Fill the memory randomly
	memory.fill_rand();

	for (int i = 0; i < access_times; i++) {
		probability = std::rand() % 101;

		if (probability <= 10) {
			index = std::rand() % memory_size;

			if (arc_cache.lookup(&memory.data[index]))
				hit_count++;
		} else if (probability <= 25) {
			index = std::rand() % static_cast<int>(0.6 * memory_size);

			if (arc_cache.lookup(&memory.data[index]))
				hit_count++;
		} else {
			index = std::rand() % static_cast<int>(0.3 * memory_size);

			if (arc_cache.lookup(&memory.data[index]))
				hit_count++;
		}
	}

	percent = ((float) hit_count) * 100.f / access_times;
	std::cout << "Unit Test 3 (Probability): hits - " << hit_count
			<< ", total amount of requests - " << access_times << " ("
			<< std::setprecision(3) << percent << "%)" << "\n";
}
