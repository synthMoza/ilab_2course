#include <iostream>

#include "cacheData.h"
#include "ARCache.h"
#include "Memory.h"

const int CACHE_SIZE = 10;
const int MEMORY_SIZE = 100;

int main() {
	cacheData<int> data(5);
	ARCache<cacheData<int>> arc_cache(CACHE_SIZE);
	Memory<cacheData<int>> memory(MEMORY_SIZE);

	memory.fill_rand();

	for (int i = 0; i < memory.size - 20; i++) {
		arc_cache.lookup(&memory.data[i]);
	}

	std::cout << "The element is " << memory.data[MEMORY_SIZE - 21].data
			<< "\n";

	arc_cache.printLists();
	arc_cache.lookup(&memory.data[MEMORY_SIZE - 21]);
	arc_cache.printLists();

	arc_cache.lookup(&memory.data[MEMORY_SIZE - 22]);
	arc_cache.printLists();

	arc_cache.lookup(&memory.data[MEMORY_SIZE - 23]);
	arc_cache.printLists();

	arc_cache.lookup(&memory.data[MEMORY_SIZE - 32]);
	arc_cache.printLists();

	return 0;
}
