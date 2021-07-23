#pragma once

#include <unordered_map>
#include <vector>
#include <string>

namespace se {
    // Forward declaration
    class BaseNode;

    enum class NameType {
        VAR, FUNC
    };

    // The value of the symbol table, contains info about this name
    struct NameInfo {
        NameType type_;

        NameInfo(NameType type) : type_ (type) {}
        virtual ~NameInfo() = default;
    };
    
    // Inherits from NameInfo, contains the value of the variable
    struct VarInfo final : public NameInfo {
        int value_;

        VarInfo() : value_{}, NameInfo(NameType::VAR) {}
        VarInfo(int value) : value_ (value), NameInfo(NameType::VAR) {}
    };

    struct FuncInfo final : public NameInfo {
        BaseNode* body_;
        std::vector<std::string> args_; 

        FuncInfo(BaseNode* body, std::vector<std::string> args) :
            NameInfo(NameType::FUNC), body_ (body), args_ (args) {}
        
        ~FuncInfo();
    };

    // Symbol table to store information about each name declaration in the scopes
    class Symtab {
        std::unordered_map<std::string, NameInfo*> table_;
    public:
        // Clear all elements from the symbol table
        void clear();
        void insert(NameInfo* info, const std::string& name);
        NameInfo* lookup(const std::string& name) const;

        ~Symtab();
    };
}