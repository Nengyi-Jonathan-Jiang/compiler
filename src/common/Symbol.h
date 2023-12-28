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

symbol make_symbol(const std::string& name) {
    static int id = 0;
    return std::make_shared<symbol_>(name, id++);
}

namespace special_symbols {
    const symbol START = make_symbol("__START__");
    const symbol END = make_symbol("__END__");
}