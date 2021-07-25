#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <memory>

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
        // Virtual method that releases all resources of this name info (like scope for FuncInfo)
        virtual void release() {}
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

        FuncInfo() : NameInfo(NameType::FUNC), body_ (nullptr) {}
        FuncInfo(BaseNode* body, std::vector<std::string> args) :
            NameInfo(NameType::FUNC), body_ (body), args_ (args) {}
        
        // Pleasantly looking setters
        void setBody(BaseNode* body) {
            body_ = body;
        }
        void setArgs(std::vector<std::string> args) {
            args_ = args;
        }
        void release() override;
        ~FuncInfo();
    };

    // Symbol table to store information about each name declaration in the scopes
    class Symtab {
        std::unordered_map<std::string, std::shared_ptr<NameInfo>> table_;
    public:
        void clear() {
            table_.clear();
        }
        // Check all the elements in the table
        void dump() const;
        void insert(std::shared_ptr<NameInfo> info, const std::string& name);
        std::shared_ptr<NameInfo> lookup(const std::string& name) const;
        void erase_name(const std::string& name) {
            table_.erase(name);
        }
        ~Symtab();
    };
}