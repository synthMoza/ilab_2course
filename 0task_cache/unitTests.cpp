#include <iostream>

#include "unitTests.h"
#include "Memory.h"
#include "cacheData.h"
#include "ARCache.h"

void unit_test_1(int cache_size, int memory_size, int access_times) {
	// For output
	int hit_count = 0;
	double percent = 0;

	ARCache<cacheData<int>> arc_cache(cache_size);
	Memory<cacheData<int>> memory(memory_size);

	// Fill the memory randomly
	memory.fill_rand();

	// Access cache 'access_times' times
	for (int i = 0; i < access_times; i++) {
		int _index = std::rand() % memory_size + 1;

		std::cout << "Current element is " << memory.data[_index].data << "\n";
		std::cout << "Current element ID is " << memory.data[_index].id << "\n";

		if (arc_cache.lookup(&memory.data[_index]))
			hit_count++;

		arc_cache.printLists();
	}

	percent = hit_count * 100 / access_times;
	std::cout.precision(4);
	std::cout << "Unit Test 1: hits - " << hit_count
			<< ", total amount of requests - " << access_times << " (" << std::fixed << percent  <<"%)" << "\n";
}
