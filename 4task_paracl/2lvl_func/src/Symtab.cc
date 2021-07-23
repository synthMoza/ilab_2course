#include "Symtab.hpp"
#include "Node.hpp"

#include <stdexcept>

using namespace se;

// Function info method
FuncInfo::~FuncInfo() {
    delete body_;
}

// SymTab methods

void Symtab::insert(NameInfo* info, const std::string& name) {
    // Lookup before inserting will give runtime error if it already exists
    if (info == nullptr)
        throw std::runtime_error("Unexpected null name info!");

    // Insert new name into the hash table
    table_[name] = info;
}

NameInfo* Symtab::lookup(const std::string& name) const {
    auto info_it = table_.find(name);
    if (info_it == table_.end())
        return nullptr;
    else
        return info_it->second;
}

void Symtab::clear() {
    // Clear hash table
    for (auto pair : table_) {
        if (pair.second != nullptr)
            delete pair.second;
    }

    table_.clear();
}

Symtab::~Symtab() {
    // Clear hash table
    for (auto pair : table_) {
        if (pair.second != nullptr)
            delete pair.second;
    }
}