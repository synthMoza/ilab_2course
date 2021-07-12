#pragma once

#include <unordered_map>
#include <string>

namespace se {
    enum NameType {
        VAR, FUNC
    };

    // The value of the symbol table, contains info about this name
    struct NameInfo {
        NameType type_;

        NameInfo(NameType type);
        virtual ~NameInfo();
    };
    
    // Inherits from NameInfo, contains the value of the variable
    struct VarInfo : public NameInfo {
        int value_;

        VarInfo();
        VarInfo(int value);
    };

    // Symbol table to store information about each name declaration in the scopes
    class Symtab {
        std::unordered_map<std::string, NameInfo*> table_;
    public:
        void insert(NameInfo* info, const std::string& name);
        NameInfo* lookup(const std::string& name);

        ~Symtab();
    };
}