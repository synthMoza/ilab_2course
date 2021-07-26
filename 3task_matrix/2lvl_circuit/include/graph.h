#pragma once

#include <vector>
#include <deque>
#include <stdexcept>

namespace detail {
    // Node color
    enum Color {
        WHITE, GREY
    };
}

namespace se {
    using path = std::vector<std::size_t>;
    using branch = std::pair<std::size_t, std::size_t> ;

    // Undirected unweighted graph class
    class Graph {
        std::vector<std::deque<std::size_t>> adjList_;
        std::vector<detail::Color> vertices_;
        // Recursive methods for finding loop
        std::size_t start_ = 0; // start node
        bool find_loop(path& loop, std::size_t parent, std::size_t index) {
            vertices_[index] = detail::GREY;
            loop.push_back(index);
            
            for (auto&& vertice : adjList_.at(index)) {
                // If the vertice isn't colored grey, scan it
                if (vertices_[vertice] == detail::WHITE) {
                    if (find_loop(loop, index, vertice)) {
                        // Found the loop
                        return true;
                    }

                } else if (vertice != parent) {
                    // Found the loop
                    loop.push_back(vertice);
                    return true;
                }
            }

            loop.pop_back();
            return false;
        }

        // Recursive method for depth first search
        void get_spanning_tree(Graph& tree, Graph& co_tree, std::size_t index) {
            for (auto&& vertice : adjList_.at(index)) {
                // If the vertice isn't colored grey, add it to the tree
                if (vertices_[vertice] == detail::WHITE) {
                    colorVertice(vertice);
                    tree.addEdge(index, vertice);
                    get_spanning_tree(tree, co_tree, vertice);
                } else if (!tree.isEdge(index, vertice) && !co_tree.isEdge(index, vertice))
                    co_tree.addEdge(index, vertice);
            }
        }
        // Color the given vertice
        void colorVertice(std::size_t vertice) {
            vertices_[vertice] = detail::GREY;
        }
        // Reset all colors to white
        void resetColors() {
            for (auto&& vertice : vertices_)
                vertice = detail::WHITE;
        }
        // Test if the given edge is in the graph
        bool isEdge(std::size_t start_vertex, std::size_t end_vertex) {
            if (start_vertex >= vertices_.size() || end_vertex >= vertices_.size())
                throw std::out_of_range("Vertices are not in this graph!");
            
            for (auto&& vertex : adjList_.at(start_vertex)) {
                if (vertex == end_vertex)
                    return true;
            }

            return false;
        }
        // Get the first edge that can be taken from the graph
        branch getEdge() {
            for (std::size_t i = 0; i < adjList_.size(); ++i)
                for (std::size_t j = 0; j < adjList_[i].size(); ++j)
                    if (!adjList_[i].empty())
                        return branch(i, adjList_[i][j]);
            
            return branch(0, 0);
        }
    public:
        // Construct adjacency list and vertices vector of the given size 
        Graph(std::size_t nvert) : adjList_(nvert), vertices_(nvert) {}
        // Get the number of edges in the graph
        std::size_t getEdges() {
            std::size_t nedges = 0;
            for (auto&& vertice : adjList_)
                nedges += vertice.size();

            return nedges / 2;
        }
        // Get the number of nodes in the graph
        std::size_t getNodes() {
            return vertices_.size();
        }
        // Add a given edge to the graph
        void addEdge(std::size_t start_vertex, std::size_t end_vertex) {
            std::size_t max_vertex = std::max(start_vertex, end_vertex);
            if (max_vertex >= vertices_.size()) {
                // Add missing vertices
                adjList_.resize(max_vertex);
                vertices_.resize(max_vertex);
            }

            adjList_[start_vertex].push_back(end_vertex);
            adjList_[end_vertex].push_back(start_vertex);
        }
        // Remove the given edge from the graph
        void removeEdge(std::size_t start_vertex, std::size_t end_vertex) {
            std::size_t max_vertex = std::max(start_vertex, end_vertex);
            if (max_vertex >= vertices_.size())
                throw std::out_of_range("Vertices are not in this graph!");
            
            auto&& start_dq = adjList_.at(start_vertex);
            auto&& end_dq = adjList_.at(end_vertex);

            for (auto it = start_dq.begin(); it != start_dq.end(); ++it) {
                if (*it == end_vertex) {
                    start_dq.erase(it);
                    break;
                }
            } 

            for (auto it = end_dq.begin(); it != end_dq.end(); ++it) {
                if (*it == start_vertex) {
                    end_dq.erase(it);
                    break;
                }
            } 
        }
        // Get a spanning tree of this graph and co-tree
        std::pair<Graph, Graph> getSpanningTree() {
            std::size_t nvertices = vertices_.size();
            Graph spanning_tree(nvertices);
            Graph co_tree(nvertices);

            colorVertice(0);
            get_spanning_tree(spanning_tree, co_tree, 0);
            resetColors();
            return std::pair<Graph, Graph>(spanning_tree, co_tree);
        }
        // Find all fundamental loops in the graph
        std::vector<path> getLoops() {
            std::size_t nloops =  getEdges() - getNodes() + 1; 
            std::pair<Graph, Graph> pair = getSpanningTree();
            // Get spanning and co_tree
            Graph& spanning_tree = pair.first;
            Graph& co_tree = pair.second;
            std::vector<path> f_loops;

            std::size_t counter = 0;
            for (std::size_t i = 0; i < co_tree.adjList_.size(); ++i) {
                for (std::size_t j = 0; j < co_tree.adjList_.at(i).size(); ++j) {
                    if (counter >= nloops)
                        return f_loops;
                    
                    Graph tmp{spanning_tree};
                    branch edge = co_tree.getEdge();
                    co_tree.removeEdge(edge.first, edge.second);

                    tmp.addEdge(edge.first, edge.second);
                    
                    path loop;
                    start_ = edge.first;
                    tmp.find_loop(loop, edge.first, edge.first);
                    f_loops.push_back(loop);
                    
                    counter++;
                    resetColors();
                }
            }

            return f_loops;
        }

        // Print the adjacency list of this graph
        void dumpAdjList() {
            std::cout << "Graph Adjacency list" << std::endl;

            for (std::size_t i = 0; i < adjList_.size(); ++i) {
                std::cout << "[" << i << "]: ";
                for (auto&& el : adjList_[i]) {
                    std::cout << el << " ";
                }
                std::cout << std::endl;
            }
        }
    };
}