#pragma once

#include <iostream>
#include <vector>

#include "Symtab.hpp"

namespace se {
    // Node structure that represents the basic class
    struct BaseNode {
        std::vector<BaseNode*> children_;

        BaseNode();

        void addChild(BaseNode* child);
        virtual int processNode(Symtab* table) = 0;
    };

    // Number node
    struct NumNode : public BaseNode {
        int value_;

        NumNode(int value);
        int processNode(Symtab* table) override;
    };

    // Variable node
    struct VarNode : public BaseNode {
        std::string name_;

        VarNode(const std::string& name);
        int processNode(Symtab* table) override;
    };

    // Operation node
    struct OpNode : public BaseNode {
        Symtab* table_;
        enum operation {
            NONE = 0, PLUS, MINUS, MUL, DIV, U_MINUS, U_PLUS, ASSIGN,
            GR, GREQ, LS, LSEQ, EQUAL, NEQUAL, COUNT
        } op_;

        OpNode(operation op);
        int processNode(Symtab* table) override;
    private:
        int process_plus();
        int process_u_plus();
        int process_u_minus();
        int process_minus();
        int process_mul();
        int process_div();
        int process_assign();
        int process_gr();
        int process_greq();
        int process_ls();
        int process_lseq();
        int process_equal();
        int process_nequal();
    };

    // Temp realisation of input and output functions
    // Console input node
    struct InputNode : public BaseNode {
        int processNode(Symtab* table) override;
    };


    // Output node
    struct OutputNode : public BaseNode {
        int processNode(Symtab* table) override;
    };
}