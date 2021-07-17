#include "Node.hpp"

#include <stack>

using namespace se;

// BaseNode methods

// Simple deallocation with casting to proper type with calling destructor
#define DEFAULT_DEALLOC(val, name)                  \
    case node_type::val:                            \
    {                                               \
        name* tmp = static_cast<name*>(this);       \
        std::cout << "tmp=" << tmp << std::endl;    \
        tmp->~name();                               \
    } break;                                        \

// Static functions for processing children during clearing
static BaseNode* getLeftChild(node_type type, BaseNode* node) {
    switch (type) {
        case node_type::BIN_OP: {
            BinOpNode* bin_op_node = static_cast<BinOpNode*>(node);
            return bin_op_node->getLeftChild();
        }
        case node_type::UN_OP: {
            UnOpNode* un_op_node = static_cast<UnOpNode*>(node);
            return un_op_node->getChild();
        }
        default:
            return nullptr;
    }
}

static BaseNode* getRightChild(node_type type, BaseNode* node) {
    if (type == node_type::BIN_OP) {
        BinOpNode* bin_op_node = static_cast<BinOpNode*>(node);
        return bin_op_node->getRightChild();
    } else
        return nullptr;
}

void BaseNode::destroy_node() {
    switch (type_) {
        DEFAULT_DEALLOC(NUM, NumNode);
        DEFAULT_DEALLOC(VAR, VarNode);
        DEFAULT_DEALLOC(SCOPE, ScopeNode);
        DEFAULT_DEALLOC(IF, IfNode);
        DEFAULT_DEALLOC(WHILE, WhileNode);
        DEFAULT_DEALLOC(UN_OP, UnOpNode);
        case node_type::BIN_OP: {
            // Delete binary tree using stack
            std::stack<BaseNode*> nodes;
            BaseNode* current = this;
            BaseNode* tmp = nullptr;

            while (current != nullptr || !nodes.empty()) {
                while (current != nullptr) {
                    nodes.push(current);
                    current = getLeftChild(current->type_, current);
                }

                // Now current is nullptr
                current = nodes.top();
                nodes.pop();
                tmp = getRightChild(current->type_, current);
                // std::cout << "current=" << current << std::endl;

                // std::cout << "HERE!" << std::endl;

                delete current;
                current = tmp;
            }
            
            break;
        }
        default:
            throw std::logic_error("Unknown node type!");
    }
}

BaseNode::~BaseNode() {
    destroy_node();
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

BinOpNode::~BinOpNode() {
    // delete left_;
    // delete right_;
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
    // if (child_ != nullptr)
    //    delete child_;
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
        delete node;
}

// IfNode methods

int IfNode::processNode() {
    int res = 0;

    if (cond_->processNode())
        res = scope_->processNode();

    return res;
}

IfNode::~IfNode() {
    delete cond_;
    delete scope_;
}

// While node methods

int WhileNode::processNode() {
    while (cond_->processNode())
        scope_->processNode();
    
    return 0;
}

WhileNode::~WhileNode() {
    delete cond_;
    delete scope_;
}