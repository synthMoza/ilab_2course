#include "Symtab.hpp"
#include "Node.hpp"

#include <stdexcept>

using namespace se;

// Function info method
FuncInfo::~FuncInfo() {
    delete body_;
}

void FuncInfo::release() {
    if (body_ != nullptr) {
        delete body_;
        body_ = nullptr;
    }
}

// SymTab methods

void Symtab::dump() const {
    std::cout << "Dump Symtab, address = " << this << std::endl;
    
    int i = 0;
    for (auto&& el : table_) {
        std::cout << "[" << i << "]: " << el.first << ", " << el.second << ", own_count=" << el.second.use_count() << std::endl;
        ++i;
    }
}

void Symtab::insert(std::shared_ptr<NameInfo> info, const std::string& name) {
    // Lookup before inserting will give runtime error if it already exists
    if (info == nullptr)
        throw std::runtime_error("Unexpected null name info!");

    // Insert new name into the hash table
    table_[name] = std::shared_ptr<NameInfo>(info);
}

std::shared_ptr<NameInfo> Symtab::lookup(const std::string& name) const {
    auto info_it = table_.find(name);
    if (info_it == table_.end())
        return nullptr;
    else
        return info_it->second;
}

Symtab::~Symtab() {
    // Release all scopes
    for (auto&& pair : table_) {
        pair.second->release();
    }
}