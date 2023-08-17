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

    static const std::shared_ptr<symbol_> START, END;
};

using symbol = std::shared_ptr<symbol_>;