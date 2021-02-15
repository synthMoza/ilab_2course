#include "set.h"

using namespace mfns;

// Find all values from the given intervals using AVL tree
std::vector<int> avlset(DataIt data_begin, DataIt data_end, ReqIt req_begin, ReqIt req_end) {
    mfns::avlset<int> set;
    std::vector<int> result;

    try {
        for (auto it = data_begin; it != data_end; ++it)
            set.insert(*it);

        int res = 0;
        for (auto it = req_begin; it != req_end; ++it) {
            res = set.distance(it->first, it->second);
            result.push_back(res);
        }
    } 
    catch (std::exception& exception) {
        std::cout << exception.what();
        throw ;
    }

    return result;
}

// Find all values from the given intervals using std::set
std::vector<int> stdset(DataIt data_begin, DataIt data_end, ReqIt req_begin, ReqIt req_end) {
    std::set<int> set;
    std::vector<int> result;

    try {
        for (auto it = data_begin; it != data_end; ++it)
            set.insert(*it);

        std::set<int>::iterator it1, it2;
        int res = 0;

        for (auto it = req_begin; it != req_end; ++it) {
            it1 = set.lower_bound(it->first);
            it2 = set.lower_bound(it->second);
            res = std::distance(it1, it2);
            if (it2 == set.find(it->second) && it2 != set.end())
                res++;

            result.push_back(res);
        }
    }
    catch (std::exception& exception) {
        std::cout << exception.what();
        throw ;
    }

    return result;
}