#pragma once

#include "Node.hpp"
#include "Symtab.hpp"

namespace se {
    // Instruction node has the pointer to the current instruction and the next one
    struct INode {
        BaseNode* node_; // instruction to do
        Symtab* table_; // the pointer to the current scope symbol table

        INode(BaseNode* node, Symtab* table);
        // Commit the instruction
        virtual void commit();
        virtual ~INode();
    };
    
    // "If" node that inherits from the instruction node
    struct IfNode : public INode {
        INode* code_;

        IfNode(BaseNode* cond, Symtab* table, INode* code);
        void commit() override;
    };
}