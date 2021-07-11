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

// OpNode methods
OpNode::OpNode(operation op) : op_(op), table_ (nullptr) {}

int OpNode::process_plus() {
    int res = 0;
    for (auto child : children_)
        res += child->processNode(table_);

    return res;   
}

int OpNode::process_minus() {
    int res = children_.at(0)->processNode(table_);
    for (auto it = std::next(children_.begin()), it_end = children_.end(); it != it_end; ++it) {
        res -= (*it)->processNode(table_);
    }
    
    return res;
}

int OpNode::process_mul() {
    int res = 1;
    for (auto child : children_) {
        res *= child->processNode(table_);
    }

    return res;
}

int OpNode::process_div() {
    int res = children_.at(0)->processNode(table_);
    int by = 0;
    for (auto it = std::next(children_.begin()), it_end = children_.end(); it != it_end; ++it) {
        by = (*it)->processNode(table_);
        if (by == 0)
            throw std::logic_error("Division by zero!");
        res /= by;
    }
    
    return res;
}

int OpNode::process_u_plus() {
    return children_.at(0)->processNode(table_);
}

int OpNode::process_u_minus() {
    return (-children_.at(0)->processNode(table_));
}

int OpNode::process_assign() {
    VarNode* var_node = static_cast<VarNode*>(children_.at(0));
    BaseNode* expr_node = children_.at(1);

    NameInfo* name_info = table_->lookup(var_node->name_);
    VarInfo* var_info = nullptr;
    if (name_info == nullptr) {
        // Declara the new variable
        table_->insert(VAR, var_node->name_);
        name_info = table_->lookup(var_node->name_);
    }
    
    var_info = static_cast<VarInfo*>(name_info);
    var_info->value_ = expr_node->processNode(table_);
    return var_info->value_;
}

int OpNode::process_gr() {
    if (children_.size() < 2)
        throw std::runtime_error("Too less arguments for the operation!");
    
    return children_.at(0)->processNode(table_) > children_.at(1)->processNode(table_);
}

int OpNode::process_greq() {
    if (children_.size() < 2)
        throw std::runtime_error("Too less arguments for the operation!");
    
    return children_.at(0)->processNode(table_) >= children_.at(1)->processNode(table_);
}

int OpNode::process_ls() {
    if (children_.size() < 2)
        throw std::runtime_error("Too less arguments for the operation!");
    
    return children_.at(0)->processNode(table_) < children_.at(1)->processNode(table_);
}

int OpNode::process_lseq() {
    if (children_.size() < 2)
        throw std::runtime_error("Too less arguments for the operation!");
    
    return children_.at(0)->processNode(table_) <= children_.at(1)->processNode(table_);
}

int OpNode::process_equal() {
    if (children_.size() < 2)
        throw std::runtime_error("Too less arguments for the operation!");
    
    return children_.at(0)->processNode(table_) == children_.at(1)->processNode(table_);
}

int OpNode::process_nequal() {
    if (children_.size() < 2)
        throw std::runtime_error("Too less arguments for the operation!");
    
    return children_.at(0)->processNode(table_) != children_.at(1)->processNode(table_);
}

int OpNode::processNode(Symtab* table) {
    if (table == nullptr)
        throw std::runtime_error("Unexpected null symbol table!");
    table_ = table;

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
        case ASSIGN:
            return process_assign();
        case GR:
            return process_gr();
        case GREQ:
            return process_greq();
        case LS:
            return process_ls();
        case LSEQ:
            return process_lseq();
        case EQUAL:
            return process_equal();
        case NEQUAL:
            return process_nequal();

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

// InputNode methods

int InputNode::processNode(Symtab* table) {
    int input = 0;
    
    std::cin >> input;
    return input;
}

// OutputNode methods

int OutputNode::processNode(Symtab* table) {
    if (children_.size() < 1)
        throw std::runtime_error("Unexpected output error!");
    
    int output = 0;
    output = children_.at(0)->processNode(table);
    std::cout << output << std::endl;
    return output;
}