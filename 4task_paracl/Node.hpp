#pragma once

#include <iostream>
#include <vector>

namespace se {
    // Node structure that represents the basic class
    struct BaseNode {
        std::vector<BaseNode*> children_;

        BaseNode();

        void addChild(BaseNode* child);
        virtual int processNode() = 0;
        virtual void dumpNode() = 0;
    };

    // Number node
    struct NumNode : public BaseNode {
        int value_;

        NumNode(int value);
        int processNode() override;
        void dumpNode() override;
    };

    // Operation node
    struct OpNode : public BaseNode {
        enum operation {
            NONE = 0, PLUS, MINUS, MUL, DIV, U_MINUS, U_PLUS, COUNT
        } op_;

        OpNode(operation op);
        int processNode() override;
        void dumpNode() override;
    private:
        int process_plus();
        int process_u_plus();
        int process_u_minus();
        int process_minus();
        int process_mul();
        int process_div();
    };
}