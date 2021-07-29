#include "Symtab.hpp"
#include "Node.hpp"

#include <stdexcept>

using namespace se;

// For better symbol table dump
std::ostream& operator<<(std::ostream& os, NameType type) {
    switch (type) {
        case NameType::VAR:
            os << "VAR";
            break;
        case NameType::FUNC:
            os << "FUNC";
            break;
        default:
            break;
    }

    return os;
}

// Function info method

void FuncInfo::release() {
    if (body_ != nullptr) {
        delete body_;
        body_ = nullptr;
    }
}

// Symtab methods

void Symtab::restore(Symtab* rhs) {
    for (auto&& pair : rhs->table_)
        table_[pair.first]->assign(pair.second.get());
    
    delete rhs;
}

void Symtab::dump() const {
    std::cout << "Dump Symtab, address = " << this << std::endl;
    
    int i = 0;
    for (auto&& el : table_) {
        std::cout << "[" << i << "]: " << el.first << ", " << el.second;
        std::cout << ", own_count=" << el.second.use_count() << ", type = " << el.second->type_;
        if (el.second->type_ == NameType::VAR)
            std::cout << ", value = " << std::static_pointer_cast<VarInfo>(el.second)->value_;
        
        std::cout << std::endl;
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