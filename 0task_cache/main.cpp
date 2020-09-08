#include <iostream>

#include "ARCache.h"

const size_t CACHE_SIZE = 10;

int main() {
	cacheData<int> data(5);
	ARCache<cacheData<int>> arc_cache(CACHE_SIZE);

	arc_cache.lookup(&data);
	arc_cache.printLists();
	arc_cache.lookup(&data);
	arc_cache.printLists();

	return 0;
}
