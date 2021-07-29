#pragma once

#include <iostream>
#include <vector>

#include "Symtab.hpp"

namespace se {
    /* Node structure that represents the basic class */

    // Node type for proper resource deleting
    enum class node_type {
        NUM, DECL, VAR, BIN_OP, UN_OP, SCOPE, IF, WHILE, RET
    };
    
    class BaseNode {
        node_type type_;

        // Helper function for proper memory deallocation
        void destroy_node();
    public:
        BaseNode(node_type type) : type_ (type) {}
        node_type getType() const {
            return type_;
        }
        virtual int processNode() = 0;
        virtual ~BaseNode() = default;
    };

    // Number node
    class NumNode final : public BaseNode {
        int value_;
    public:
        NumNode(int value) : BaseNode(node_type::NUM), value_ (value) {}
        int processNode() override {
            return value_;
        }
        ~NumNode() = default;
    };

    // Declaration node
    class DeclNode : public BaseNode {
    protected:
        std::string name_;
        std::shared_ptr<NameInfo> name_info_;
    public:
        DeclNode(const std::string& name, std::shared_ptr<NameInfo> name_info, node_type type = node_type::DECL) : 
            BaseNode(type), name_ (name), name_info_ (name_info) {}
        virtual ~DeclNode() = default;
    };
    
    // Variable node that inherits from DeclNode and has the "setValue()" method
    class VarNode final : public DeclNode {
    public:
        VarNode(const std::string& name, std::shared_ptr<VarInfo> var_info) : DeclNode(name, var_info, node_type::VAR) {}
        int processNode() override;
        void setValue(int value);

        ~VarNode() = default;
    };

    // Function node that contains its body as a pointer to scope node
    class FuncNode final : public DeclNode {
        std::vector<BaseNode*> args_;
    public:
        FuncNode(const std::string& name, std::shared_ptr<FuncInfo> func_info, std::vector<BaseNode*> args) 
            : DeclNode(name, func_info), args_ (args) {}

        int processNode() override;

        ~FuncNode();
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

        void erase_name(const std::string& name) {
            table_->erase_name(name);
        }
        void dump() const {
            table_->dump();
        }
        // Get exact copy of current table, used for restoreTable() method
        Symtab* getTableCopy() {
            return new Symtab(*table_);
        }
        // Restores current table with values from the given one (deletes given table)
        void restoreTable(Symtab* table) {
            table_->restore(table);
        }

        // Wrappers for symbol table methods
        void clear() {
            table_->clear();
        }
        void insert(std::shared_ptr<NameInfo> info, const std::string& name);
        std::shared_ptr<NameInfo> lookup(const std::string& name) const;
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

    // Exception-wrapper for return codes inside functions 
    class ret_exception {
        int code_;
    public:
        ret_exception(int code) : code_ (code) {}
        const int& get() const {
            return code_;
        }
    };

    // Return node for functions to return value any time
    class RetNode final : public BaseNode {
        BaseNode* expression_;
    public:
        RetNode(BaseNode* expression) : BaseNode(node_type::RET), expression_ (expression) {}
        int processNode() override {
            throw ret_exception(expression_->processNode());
        }

        ~RetNode();
    };
}