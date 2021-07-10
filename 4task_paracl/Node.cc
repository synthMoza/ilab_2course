#include "Node.hpp"

using namespace se;

// Base node methods
BaseNode::BaseNode() : 
    children_{} {}

void BaseNode::addChild(BaseNode* child) {
    if (child == nullptr)
        throw std::runtime_error("Unexpected nullptr in Node!");
    
    children_.push_back(child);
}

// NumNode methods
NumNode::NumNode(int value) : 
    value_ (value) {}

int NumNode::processNode() {
    return value_;
}

void NumNode::dumpNode() {
    std::cout << value_;
}

// OpNode methods
OpNode::OpNode(operation op) : op_(op) {}

int OpNode::process_plus() {
    int res = 0;
    for (auto child : children_)
        res += child->processNode();

    return res;   
}

int OpNode::process_minus() {
    int res = children_.at(0)->processNode();
    for (auto it = std::next(children_.begin()), it_end = children_.end(); it != it_end; ++it) {
        res -= (*it)->processNode();
    }
    
    return res;
}

int OpNode::process_mul() {
    int res = 1;
    for (auto child : children_) {
        res *= child->processNode();
    }

    return res;
}

int OpNode::process_div() {
    int res = children_.at(0)->processNode();
    int by = 0;
    for (auto it = std::next(children_.begin()), it_end = children_.end(); it != it_end; ++it) {
        by = (*it)->processNode();
        if (by == 0)
            throw std::logic_error("Division by zero!");
        res /= by;
    }
    
    return res;
}

int OpNode::process_u_plus() {
    return children_.at(0)->processNode();
}

int OpNode::process_u_minus() {
    return (-children_.at(0)->processNode());
}

int OpNode::processNode() {
    switch (op_) {
        case PLUS:
            return process_plus();
        case MINUS:
            return process_minus();
        case MUL:
            return process_mul();
        case DIV:
            return process_div();
        case U_PLUS:
            return process_u_plus();
        case U_MINUS:
            return process_u_minus();
        default:
            throw std::runtime_error("Failed to recognize the operation " + std::to_string(op_));
    }
}

void OpNode::dumpNode() {
    std::cout << "OP ";
    switch (op_) {
        case PLUS:
            std::cout << "+";
            break;
        case MINUS:
            std::cout << "-";
            break;
        case MUL:
            std::cout << "*";
            break;
        case DIV:
            std::cout << "/";
            break;
        case U_PLUS:
            break;
        case U_MINUS:
            std::cout << "-(u)";
            break;
        default:
            throw std::runtime_error("Failed to recognize the operation " + std::to_string(op_));
    }
}