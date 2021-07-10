#pragma once

#include "Node.hpp"

namespace se {
    // Instruction node has the pointer to the current instruction and the next one
    struct INode {
        BaseNode* node_;

        INode(BaseNode* node);
        // Commit the instruction
        void commit();
    };
}