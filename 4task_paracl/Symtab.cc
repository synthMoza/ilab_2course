#include "Symtab.hpp"

using namespace se;

// NameInfo methods
NameInfo::NameInfo(NameType type) : type_ (type) {}

NameInfo::~NameInfo() {}

// VarInfo methods
VarInfo::VarInfo(int value) : value_ (value), NameInfo(VAR) {}

VarInfo::VarInfo() : value_{}, NameInfo(VAR) {}
// SymTab methods

Symtab::Symtab() : parent_ (nullptr) {}

Symtab::Symtab(Symtab* parent) : parent_ (parent) {}

void Symtab::insert(NameType type, const std::string& name) {
    if (lookup(name) != nullptr) {
        // The element is already in this table
        throw std::runtime_error("The variable " + name + "is already in this scope!");
    }

    // Insert new name into the hash table
    VarInfo* info = nullptr;
    switch (type) {
        case VAR:
            info = new VarInfo;
            table_[name] = info;
            break;
        default:
            throw std::runtime_error("Unknown name type!");
    }
}

NameInfo* Symtab::lookup(const std::string& name) {
    auto info_it = table_.find(name);
    // Look up in the current table
    if (info_it != table_.end())
        return info_it->second;

    // If it wasn't found and there is no any parent, return 0
    if (parent_ == nullptr)
        return nullptr;

    // Look up in the parent symbol tables
    Symtab* symtable = parent_; 
    while ((info_it = symtable->table_.find(name)) == symtable->table_.end()) {
        symtable = symtable->parent_;
        if (symtable == nullptr)
            return nullptr;
    }

    return info_it->second;
}

Symtab* Symtab::getParent() {
    return parent_;
}

Symtab::~Symtab() {
    // Clear hash table
    for (auto pair : table_) {
        if (pair.second != nullptr)
            delete pair.second;
    }
}