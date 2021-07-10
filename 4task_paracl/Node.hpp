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
        virtual void dumpNode() = 0;
    };

    // Number node
    struct NumNode : public BaseNode {
        int value_;

        NumNode(int value);
        int processNode(Symtab* table) override;
        void dumpNode() override;
    };

    // Variable node
    struct VarNode : public BaseNode {
        std::string name_;

        VarNode(const std::string& name);
        int processNode(Symtab* table) override;
        void dumpNode() override;
    };

    // Operation node
    struct OpNode : public BaseNode {
        enum operation {
            NONE = 0, PLUS, MINUS, MUL, DIV, U_MINUS, U_PLUS, ASSIGN, COUNT
        } op_;

        OpNode(operation op);
        int processNode(Symtab* table) override;
        void dumpNode() override;
    private:
        int process_plus(Symtab* table);
        int process_u_plus(Symtab* table);
        int process_u_minus(Symtab* table);
        int process_minus(Symtab* table);
        int process_mul(Symtab* table);
        int process_div(Symtab* table);
        int process_assign(Symtab* table);
    };
}