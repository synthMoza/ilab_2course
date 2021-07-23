#include "Node.hpp"

#include <stack>

using namespace se;

// Static methods
/* Helper functions for proper cleaning */
static BaseNode* getLeftChild(BaseNode* node) {
    node_type type = node->getType();
    
    switch (type) {
        case node_type::BIN_OP: {
            BinOpNode* bin_op_node = static_cast<BinOpNode*>(node);
            return bin_op_node->getLeftChild();
        }
        case node_type::UN_OP: {
            // UnOpNode'child is considered as left
            UnOpNode* un_op_node = static_cast<UnOpNode*>(node);
            return un_op_node->getChild();
        }
        default:
            // No children
            return nullptr;
    }
}

static BaseNode* getRightChild(BaseNode* node) {
    node_type type = node->getType();

    if (type == node_type::BIN_OP) {
        BinOpNode* bin_op_node = static_cast<BinOpNode*>(node);
        return bin_op_node->getRightChild();
    } else
        return nullptr;
}

// Static function for deleting the whole tree using stack (used in ScopeNode, WhileNode, IfNode, ...)
static void delete_tree(BaseNode* node) {
    node_type type;
    
    type = node->getType();
    if (type != node_type::BIN_OP) {
        // No need to delete children (UnOpNode considered to be not too heavy to be recursively deleted)
        delete node; 
    } else {
        std::stack<BaseNode*> nodes;
        BaseNode* current = node;
        BaseNode* tmp = nullptr;

        while (current != nullptr || !nodes.empty()) {
            while (current != nullptr) {
                nodes.push(current);
                current = getLeftChild(current);
            }

            current = nodes.top();
            nodes.pop();
            tmp = getRightChild(current);

            delete current;

            current = tmp;
        }
    }
}

// VarNode methods

void VarNode::setValue(int value) {
    VarInfo* var_info = static_cast<VarInfo*>(name_info_);
    var_info->value_ = value;
}

int VarNode::processNode() {
    VarInfo* var_info = static_cast<VarInfo*>(name_info_);
    return var_info->value_;
}

// Binary Operation Node

#define CASE_BIN_OP(name, op)                                       \
    case bin_op_type::name:                                         \
        return (left_->processNode() op right_->processNode());     \

int BinOpNode::processNode() {
    if (left_ == nullptr || right_ == nullptr)
        throw std::runtime_error("Unexpected null children!");
    
    int value = 0;
    VarNode* lvalue = nullptr;
    
    switch (type_) {
        CASE_BIN_OP(PLUS, +);
        CASE_BIN_OP(MINUS, -);
        CASE_BIN_OP(MUL, *);
        CASE_BIN_OP(MOD, %);
        CASE_BIN_OP(GR, >);
        CASE_BIN_OP(GREQ, >=);
        CASE_BIN_OP(LS, <);
        CASE_BIN_OP(LSEQ, <);
        CASE_BIN_OP(EQUAL, ==);
        CASE_BIN_OP(NEQUAL, !=);
        CASE_BIN_OP(L_AND, &&);
        CASE_BIN_OP(L_OR, ||);
        case bin_op_type::DIV:
            value = right_->processNode();
            if (value == 0)
                throw std::logic_error("Division by zero!");
            return left_->processNode() / value;
        case bin_op_type::ASSIGN:
            lvalue = static_cast<VarNode*>(left_);
            value = right_->processNode();
            lvalue->setValue(value);
            return value;
        default:
            throw std::runtime_error("Unknown binary operation type!");
    }
}

// Unary operation node

int UnOpNode::processNode() {
    int value = 0;
    switch (type_) {
        case un_op_type::U_PLUS:
            return child_->processNode();
        case un_op_type::U_MINUS:
            return (-child_->processNode());
        case un_op_type::NOT:
            return (!child_->processNode());
        case un_op_type::INPUT:
            std::cin >> value;
            return value;
        case un_op_type::OUTPUT:
            value = child_->processNode(); 
            std::cout << value << std::endl;
            return value;
        default:
            throw std::runtime_error("Unknown unary operator type!");
    }
}

UnOpNode::~UnOpNode() {
    if (child_ != nullptr)
       delete_tree(child_);
}

// Scope node methods

int ScopeNode::processNode() {
    int result = 0;

    for (auto* node : instructions_) {
        // Commit each node
        result = node->processNode();
    }

    return result;
}

void ScopeNode::insert(NameInfo* info, const std::string& name) {
    // If it already exists, runtime error while PARSING
    if (table_ == nullptr)
        throw std::runtime_error("Unexpected null symbol table!");
    
    table_->insert(info, name);
}

NameInfo* ScopeNode::lookup(const std::string& name) const {
    auto look_scope = this;
    NameInfo* info_p = nullptr;

    do {
        info_p = look_scope->table_->lookup(name);
        if (info_p != nullptr)
            return info_p;
    } while ((look_scope = look_scope->prev_) != nullptr);


    return nullptr;
}

void ScopeNode::addInstruction(BaseNode* node) {
    instructions_.push_back(node);
}

ScopeNode::~ScopeNode() {
    delete table_;
    for (auto* node : instructions_)
        delete_tree(node);
}

// IfNode methods

int IfNode::processNode() {
    int res = 0;

    if (cond_->processNode())
        res = scope_->processNode();

    return res;
}

IfNode::~IfNode() {
    delete_tree(cond_);
    delete_tree(scope_);
}

// While node methods

int WhileNode::processNode() {
    while (cond_->processNode())
        scope_->processNode();
    
    return 0;
}

WhileNode::~WhileNode() {
    delete_tree(cond_);
    delete_tree(scope_);
}