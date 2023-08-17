#pragma once

#include "../common/token.h"

class symbolString {
    std::vector<symbol> symbols;
    size_t length;
    std::string repr;

public:
    symbolString(const std::vector<symbol>& symbols);
    symbolString(std::initializer_list<symbol> symbols);

    [[nodiscard]] size_t size() const;

    [[nodiscard]] symbol operator[](size_t i) const;
    [[nodiscard]] symbol first() const;
    [[nodiscard]] symbol last() const;

    [[nodiscard]] std::strong_ordering operator<=>(const symbolString &other) const;

    [[nodiscard]] symbolString operator()(size_t start) const;
    [[nodiscard]] symbolString operator()(size_t start, size_t end) const;

    [[nodiscard]] symbolString operator+(const symbol& sym) const;

    [[nodiscard]] std::vector<symbol>::const_iterator begin() const;
    [[nodiscard]] std::vector<symbol>::const_iterator end() const;
};