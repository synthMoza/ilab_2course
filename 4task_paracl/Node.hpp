#pragma once

#include <iostream>
#include <vector>

#include "Symtab.hpp"

namespace se {
    /* Node structure that represents the basic class */

    // Node type for more proper resource deleting
    enum class node_type {
        NUM, DECL, VAR, BIN_OP, UN_OP, SCOPE, IF, WHILE
    };
    
    class BaseNode {
        node_type type_;
        // Helper function for proper memory deallocation
        void destroy_node();
    public:
        BaseNode(node_type type) : type_ (type) {}
        node_type getType() {
            return type_;
        }
        virtual int processNode() = 0;
        virtual ~BaseNode() {}
    };

    // Number node
    class NumNode final : public BaseNode {
        int value_;
    public:
        NumNode(int value) : BaseNode(node_type::NUM), value_ (value) {}
        int processNode() override {
            return value_;
        }
        ~NumNode() {}
    };

    // Declaration node
    class DeclNode : public BaseNode {
    protected:
        std::string name_;
        NameInfo* name_info_;
    public:
        DeclNode(const std::string& name, NameInfo* name_info, node_type type = node_type::DECL) : 
            BaseNode(type), name_ (name), name_info_ (name_info) {}
        virtual ~DeclNode() {}
    };
    
    // Variable node that inherits from DeclNode and has the "setValue()" method
    class VarNode final : public DeclNode {
    public:
        VarNode(const std::string& name, VarInfo* var_info) : DeclNode(name, var_info, node_type::VAR) {}
        int processNode() override;
        void setValue(int value);

        ~VarNode() {}
    };

    // Binary operation node
    enum class bin_op_type {
        PLUS, MINUS, MUL, DIV, GR, ASSIGN, L_AND,
        L_OR, GREQ, LS, LSEQ, EQUAL, NEQUAL, MOD
    }; 
    
    class BinOpNode final : public BaseNode {
        BaseNode* left_;
        BaseNode* right_;
        bin_op_type type_;
    public:
        BinOpNode(bin_op_type type, BaseNode* left, BaseNode* right) : 
            BaseNode(node_type::BIN_OP), type_ (type), left_ (left), right_ (right) {}
        int processNode() override;
        BaseNode* getLeftChild() {
            return left_;
        }
        BaseNode* getRightChild() {
            return right_;
        }
        ~BinOpNode() {}
    };

    // Unary operation node
    enum class un_op_type {
        U_PLUS, U_MINUS, NOT, INPUT, OUTPUT
    }; 
    
    class UnOpNode final : public BaseNode {
        BaseNode* child_;
        un_op_type type_;
    public:
        UnOpNode(un_op_type type, BaseNode* child) : 
            BaseNode(node_type::UN_OP), type_ (type), child_ (child) {}
        int processNode() override;
        BaseNode* getChild() {
            return child_;
        }

        ~UnOpNode();
    };

    // Scope node - the main part of the program
    class ScopeNode final : public BaseNode {
        std::vector<BaseNode*> instructions_;
        ScopeNode* prev_;
        Symtab* table_;
    public:
        ScopeNode(ScopeNode* prev) : 
            BaseNode(node_type::SCOPE), prev_ (prev), table_ (new Symtab) {}

        // Wrappers for symbol table methods
        void insert(NameInfo* info, const std::string& name);
        NameInfo* lookup(const std::string& name) const;
        // Insert new instruction
        void addInstruction(BaseNode* node);
        // Get the previous scope
        ScopeNode* getPrevScope() const {
            return prev_;
        }

        int processNode() override;

        ~ScopeNode();
    };

    // "If" operator node
    class IfNode final : public BaseNode {
        BaseNode* cond_;
        BaseNode* scope_;
    public:
        IfNode(BaseNode* cond, BaseNode* scope) : 
            BaseNode(node_type::IF), cond_ (cond), scope_ (scope) {}
        int processNode() override;

        ~IfNode();
    };

    // "While" operator node
    class WhileNode final : public BaseNode {
        BaseNode* cond_;
        BaseNode* scope_;
    public:
        WhileNode(BaseNode* cond, BaseNode* scope) :
            BaseNode(node_type::WHILE), cond_ (cond), scope_ (scope) {}

        int processNode() override;

        ~WhileNode();
    };
}