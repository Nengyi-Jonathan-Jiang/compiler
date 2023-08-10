#pragma once

#include <string>
#include <memory>

struct symbol_ {
    std::string name;
    size_t id;

    [[nodiscard]] explicit operator const std::string&() const {
        return name;
    }

    symbol_(std::string name, int id) : name(std::move(name)), id(id) {};
};

using symbol = std::shared_ptr<symbol_>;

//std::strong_ordering operator<=>(const symbol& a, const symbol& b){
//    return a->id <=> b->id;
//}