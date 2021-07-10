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

int NumNode::processNode(Symtab* table) {
    return value_;
}

void NumNode::dumpNode() {
    std::cout << value_;
}

// OpNode methods
OpNode::OpNode(operation op) : op_(op) {}

int OpNode::process_plus(Symtab* table) {
    int res = 0;
    for (auto child : children_)
        res += child->processNode(table);

    return res;   
}

int OpNode::process_minus(Symtab* table) {
    int res = children_.at(0)->processNode(table);
    for (auto it = std::next(children_.begin()), it_end = children_.end(); it != it_end; ++it) {
        res -= (*it)->processNode(table);
    }
    
    return res;
}

int OpNode::process_mul(Symtab* table) {
    int res = 1;
    for (auto child : children_) {
        res *= child->processNode(table);
    }

    return res;
}

int OpNode::process_div(Symtab* table) {
    int res = children_.at(0)->processNode(table);
    int by = 0;
    for (auto it = std::next(children_.begin()), it_end = children_.end(); it != it_end; ++it) {
        by = (*it)->processNode(table);
        if (by == 0)
            throw std::logic_error("Division by zero!");
        res /= by;
    }
    
    return res;
}

int OpNode::process_u_plus(Symtab* table) {
    return children_.at(0)->processNode(table);
}

int OpNode::process_u_minus(Symtab* table) {
    return (-children_.at(0)->processNode(table));
}

int OpNode::process_assign(Symtab* table) {
    VarNode* var_node = static_cast<VarNode*>(children_.at(0));
    BaseNode* expr_node = children_.at(1);

    NameInfo* name_info = table->lookup(var_node->name_);
    VarInfo* var_info = nullptr;
    if (name_info == nullptr) {
        // Declara the new variable
        table->insert(VAR, var_node->name_);
        name_info = table->lookup(var_node->name_);
    }
    
    var_info = static_cast<VarInfo*>(name_info);
    var_info->value_ = expr_node->processNode(table);
    return var_info->value_;
}

int OpNode::processNode(Symtab* table) {
    switch (op_) {
        case PLUS:
            return process_plus(table);
        case MINUS:
            return process_minus(table);
        case MUL:
            return process_mul(table);
        case DIV:
            return process_div(table);
        case U_PLUS:
            return process_u_plus(table);
        case U_MINUS:
            return process_u_minus(table);
        case ASSIGN:
            return process_assign(table);
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
        case ASSIGN:    
            std::cout << "=";
            break;
        default:
            throw std::runtime_error("Failed to recognize the operation " + std::to_string(op_));
    }
}

// VarNode methods

VarNode::VarNode(const std::string& name) : name_ (name) {}

int VarNode::processNode(Symtab* table) {
    NameInfo* name_info = table->lookup(name_);
    if (name_info == nullptr)
        throw std::runtime_error("Variable " + name_ + " wasn't declared in this scope");

    VarInfo* var_info = static_cast<VarInfo*>(name_info);
    return var_info->value_;
}

void VarNode::dumpNode() {
    std::cout << name_;
}