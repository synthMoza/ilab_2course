#include "Node.hpp"

using namespace se;

// Base Node methods
BaseNode::~BaseNode() {}

// NumNode methods
NumNode::NumNode(int value) : 
    value_ (value) {}

int NumNode::processNode() {
    return value_;
}

// DeclNode methods

DeclNode::DeclNode(const std::string& name, NameInfo* name_info) : name_ (name), name_info_ (name_info) {}

int DeclNode::processNode() {
    // Nothing
    return -1;
}

DeclNode::~DeclNode() {}

// VarNode methods

VarNode::VarNode(const std::string& name, VarInfo* var_info) : DeclNode(name, var_info) {}

void VarNode::setValue(int value) {
    VarInfo* var_info = static_cast<VarInfo*>(name_info_);
    var_info->value_ = value;
}

int VarNode::processNode() {
    // Nothing
    VarInfo* var_info = static_cast<VarInfo*>(name_info_);
    return var_info->value_;
}

// Binary Operation Node

BinOpNode::BinOpNode(bin_op_type type, BaseNode* left, BaseNode* right) : type_ (type), left_ (left), right_ (right) {}

#define CASE_BIN_OP(name, op)                                       \
    case (name):                                                    \
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
        case DIV:
            value = right_->processNode();
            if (value == 0)
                throw std::logic_error("Division by zero!");
            return left_->processNode() / value;
        case ASSIGN:
            lvalue = static_cast<VarNode*>(left_);
            value = right_->processNode();
            lvalue->setValue(value);
            return value;
        default:
            throw std::runtime_error("Unknown binary operation type!");
    }
}

BinOpNode::~BinOpNode() {
    delete left_;
    delete right_;
}

// Unary operation node

UnOpNode::UnOpNode(un_op_type type, BaseNode* child) : type_ (type), child_ (child) {}

int UnOpNode::processNode() {
    int value = 0;
    switch (type_) {
        case U_PLUS:
            return child_->processNode();
        case U_MINUS:
            return (-child_->processNode());
        case NOT:
            return (!child_->processNode());
        case INPUT:
            std::cin >> value;
            return value;
        case OUTPUT:
            value = child_->processNode(); 
            std::cout << value << std::endl;
            return value;
        default:
            throw std::runtime_error("Unknown unary operator type!");
    }
}

UnOpNode::~UnOpNode() {
    if (child_ != nullptr)
        delete child_;
}

// Scope node methods

ScopeNode::ScopeNode(ScopeNode* prev) : prev_ (prev), table_ (new Symtab) {}

int ScopeNode::processNode() {
    int result = 0;

    for (auto node : instructions_) {
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

NameInfo* ScopeNode::lookup(const std::string& name) {
    ScopeNode* look_scope = this;
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

ScopeNode* ScopeNode::getPrevScope() {
    return prev_;
}

ScopeNode::~ScopeNode() {
    delete table_;

    for (auto node : instructions_)
        delete node;
}

// IfNode methods

IfNode::IfNode(BaseNode* cond, BaseNode* scope) : cond_ (cond), scope_ (scope) {}

int IfNode::processNode() {
    if (cond_->processNode())
        scope_->processNode();

    return 0;
}

IfNode::~IfNode() {
    delete cond_;
    delete scope_;
}

// While node methods

WhileNode::WhileNode(BaseNode* cond, BaseNode* scope) : cond_ (cond), scope_ (scope) {}

int WhileNode::processNode() {
    while (cond_->processNode())
        scope_->processNode();
    
    return 0;
}

WhileNode::~WhileNode() {
    delete cond_;
    delete scope_;
}