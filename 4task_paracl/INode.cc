#include "INode.hpp"

using namespace se;

INode::INode(BaseNode* node) : node_ (node) {}

void INode::commit() {
    int result = 0;

    if (node_ == nullptr)
        throw std::runtime_error("Unexpected null node during commit!");

    result = node_->processNode();
    std::cout << result << std::endl;
}