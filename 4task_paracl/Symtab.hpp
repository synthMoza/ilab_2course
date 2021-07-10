#pragma once

#include <unordered_map>
#include <string>

namespace se {
    enum NameType {
        NONE = 0, VAR, COUNT
    };

    // The value of the symbol table, contains info about this name
    struct NameInfo {
        NameType type_;

        NameInfo(NameType type);
        virtual ~NameInfo();
    };

    struct VarInfo : public NameInfo {
        int value_;

        VarInfo();
        VarInfo(int value);
    };
    
    // Symbol table to store information about each name declaration in the program
    class Symtab {
        std::unordered_map<std::string, NameInfo*> table_;
        Symtab* parent_; // parent symbol table (inner scope)

    public:
        Symtab();
        Symtab(Symtab* parent);

        void insert(NameType type, const std::string& name);
        NameInfo* lookup(const std::string& name);
        Symtab* getParent();
    };
}