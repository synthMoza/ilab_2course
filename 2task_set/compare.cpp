#include "set.h"
#ifdef GTESTS
#include "tests.h"
#endif

#ifdef COMPARE
int main() {
    int n = 0;
    int m = 0;
    int input = 0;
    std::pair<int, int> request;
    std::vector<int> data;
    std::vector<int> result;
    std::vector<std::pair<int, int>> requests;

    // Input set's data
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        std::cin >> input;
        data.push_back(input);
    }

    // Input requests
    std::cin >> m;
    for (int i = 0 ; i < m; ++i) {
        std::cin >> request.first >> request.second;
        requests.push_back(request);
    }

    std::cout << "Total elements: " << n << std::endl;
    std::cout << "Total requests: " << m << std::endl;

    //=================================================
    auto start = std::chrono::system_clock::now();

    result = stdset(data.begin(), data.end(), requests.begin(), requests.end());

    auto end = std::chrono::system_clock::now();
    //=================================================
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "STDSET, elapsed time: " << elapsed.count() << "s" << std::endl;

    //=================================================
    start = std::chrono::system_clock::now();
    
    result = avlset(data.begin(), data.end(), requests.begin(), requests.end());
    // for (auto elem : result) {
    //     std::cout << elem << " ";
    // }
    // std::cout << std::endl;
    
    end = std::chrono::system_clock::now();
    //=================================================

    elapsed = end - start;
    std::cout << "AVLSET, elapsed time: " << elapsed.count() << "s" << std::endl;

    return EXIT_SUCCESS;
}
#endif

#ifdef GTESTS
int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
#endif