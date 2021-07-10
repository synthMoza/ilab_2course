#include "INode.hpp"

using namespace se;

INode::INode(BaseNode* node, Symtab* table) : node_ (node), table_ (table) {}

void INode::commit() {
    int result = 0;

    if (node_ == nullptr)
        throw std::runtime_error("Unexpected null node during commit!");

    result = node_->processNode(table_);
    std::cout << result << std::endl;
}