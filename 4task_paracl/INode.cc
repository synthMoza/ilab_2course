#include "INode.hpp"

#include <queue>

using namespace se;

INode::INode(BaseNode* node, Symtab* table) : node_ (node), table_ (table) {}

void INode::commit() {
    int result = 0;

    if (node_ == nullptr)
        throw std::runtime_error("Unexpected null node during commit!");

    result = node_->processNode(table_);
    std::cout << result << std::endl;
}

INode::~INode() {
    // Scope will be deleted in other place
    // Delete expression tree
    BaseNode* tmp = nullptr;
    std::queue<BaseNode*> queue;
    queue.push(node_);

    while (!queue.empty()) {
        tmp = queue.front();
        queue.pop();

        for (auto child : tmp->children_) {
            if (child != nullptr)
                queue.push(child);
        }

        delete tmp;
    }
}