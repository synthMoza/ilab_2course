#pragma once

#include <unordered_map>
#include <string>

namespace se {
    enum class NameType {
        VAR
    };

    // The value of the symbol table, contains info about this name
    struct NameInfo {
        NameType type_;

        NameInfo(NameType type) : type_ (type) {}
        virtual ~NameInfo() {}
    };
    
    // Inherits from NameInfo, contains the value of the variable
    struct VarInfo : public NameInfo {
        int value_;

        VarInfo() : value_{}, NameInfo(NameType::VAR) {}
        VarInfo(int value) : value_ (value), NameInfo(NameType::VAR) {}
    };

    // Symbol table to store information about each name declaration in the scopes
    class Symtab {
        std::unordered_map<std::string, NameInfo*> table_;
    public:
        void insert(NameInfo* info, const std::string& name);
        NameInfo* lookup(const std::string& name) const;

        ~Symtab();
    };
}