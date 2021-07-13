#pragma once

#include <iostream>
#include <vector>

#include "Symtab.hpp"

namespace se {
    // Node structure that represents the basic class
    class BaseNode {
    public:
        virtual int processNode() = 0;
        virtual ~BaseNode();
    };

    // Number node
    class NumNode final : public BaseNode {
        int value_;
    public:
        NumNode(int value);
        int processNode() override;
    };

    // Declaration node
    class DeclNode : public BaseNode {
    protected:
        std::string name_;
        NameInfo* name_info_;
    public:
        DeclNode(const std::string& name, NameInfo* name_info);
        int processNode() override;

        virtual ~DeclNode();
    };
    
    // Variable node that inherits from DeclNode and has the "setValue()" method
    class VarNode final : public DeclNode {
        public:
            VarNode(const std::string& name, VarInfo* var_info);
            int processNode() override;
            void setValue(int value);
    };

    // Binary operation node
    enum bin_op_type {
        PLUS, MINUS, MUL, DIV, GR, ASSIGN, L_AND,
        L_OR, GREQ, LS, LSEQ, EQUAL, NEQUAL, MOD
    }; 
    
    class BinOpNode final : public BaseNode {
        BaseNode* left_;
        BaseNode* right_;
        bin_op_type type_;
    public:
        BinOpNode(bin_op_type type, BaseNode* left, BaseNode* right);
        int processNode() override;

        ~BinOpNode();
    };

    // Unary operation node
    enum un_op_type {
        U_PLUS, U_MINUS, NOT, INPUT, OUTPUT
    }; 
    
    class UnOpNode final : public BaseNode {
        BaseNode* child_;
        un_op_type type_;
    public:
        UnOpNode(un_op_type type, BaseNode* child);
        int processNode() override;

        ~UnOpNode();
    };

    // Scope node - the main part of the program
    class ScopeNode final : public BaseNode {
        std::vector<BaseNode*> instructions_;
        ScopeNode* prev_;
        Symtab* table_;
    public:
        ScopeNode(ScopeNode* prev);

        // Wrappers for symbol table methods
        void insert(NameInfo* info, const std::string& name);
        NameInfo* lookup(const std::string& name);
        // Insert new instruction
        void addInstruction(BaseNode* node);
        // Get the previous scope
        ScopeNode* getPrevScope();

        int processNode() override;

        ~ScopeNode();
    };

    // "If" operator node
    class IfNode final : public BaseNode {
        BaseNode* cond_;
        BaseNode* scope_;
    public:
        IfNode(BaseNode* cond, BaseNode* scope);

        int processNode() override;

        ~IfNode();
    };

    // "While" operator node
    class WhileNode final : public BaseNode {
        BaseNode* cond_;
        BaseNode* scope_;
    public:
        WhileNode(BaseNode* cond, BaseNode* scope);

        int processNode() override;

        ~WhileNode();
    };
}