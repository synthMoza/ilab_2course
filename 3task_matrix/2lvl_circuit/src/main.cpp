#include <iostream>
#include <utility>

#include "driver.h"
#include "matrix.h"
#include "graph.h"

using namespace se;

// Driver's usings
using yy::edge_t;
using yy::msize_type;

std::size_t countVertices(const std::vector<edge_t>& vector) {
    std::size_t nvertices = 0;

    // Count vertices
    for (auto&& pair : vector) {
        nvertices = std::max(pair.first.first, nvertices);
        nvertices = std::max(pair.first.second, nvertices);
    }

    return nvertices;
}

int main() {
	yy::Driver driver;
    driver.parse();

    std::vector<edge_t> vector = driver.getVector();
    std::size_t nedges = vector.size();
    std::size_t nvertices = countVertices(vector);

    Graph graph(nvertices);
    for (auto&& pair : vector)
        graph.addEdge(pair.first.first - 1, pair.first.second - 1);

    // Construct el. force and resistance matrices
    Matrix<double> Z_b(nedges, nedges);
    for (msize_type i = 0; i < nedges; ++i)
        Z_b[i][i] = vector.at(i).second.resistance_;
    Matrix<double> E_gb(nedges, 1);
    for (msize_type i = 0; i < nedges; ++i)
        E_gb[i][0] = vector.at(i).second.voltage_;

    std::size_t nloops = nedges - nvertices + 1;
    auto loops = graph.getLoops();

    Matrix<double> B(nloops, nedges);
    branch edge{};
    std::size_t j = 0;

    for (msize_type i = 0; i < nloops; ++i) {
        // Traverse i loop
        for (msize_type k = 0; k < loops[i].size() - 1; ++k) {
            edge = {loops[i][k] + 1, loops[i][k + 1] + 1};
            int res = 0;
            if (edge.second > edge.first)
                res = 1;
            else
                res = -1;

            // Traverse vector to find the number of this edge
            for (j = 0; j < nedges; ++j) {
                if (edge == vector.at(j).first)
                    break;
                std::swap(edge.first, edge.second);
                if (edge == vector.at(j).first)
                    break;
            }

            B[i][j] = res;
        }
    }

    Matrix<double> Z_m = B * Z_b;
    Z_m *= B.transpose();
    Matrix<double> E_gm = B.transpose() * E_gb;
    Matrix<double> I_m = Z_m.inverse() * E_gm;
    Matrix<double> I_b = B.transpose() * I_m;

    for (std::size_t i = 0; i < nedges; ++i) {
        auto&& edge = vector[i].first;
        std::cout << edge.first << " -- " << edge.second << ": ";
        std::cout << I_b[i][0] << " A" << std::endl;
    }

    return EXIT_SUCCESS;
}