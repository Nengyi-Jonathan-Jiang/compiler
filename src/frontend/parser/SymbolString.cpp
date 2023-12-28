#include "SymbolString.h"

#include <utility>


SymbolString::SymbolString(const std::vector<symbol>& symbols) : length(symbols.size()), symbols(symbols) {}

SymbolString::SymbolString(std::initializer_list<symbol> symbols) :
    symbols(symbols.begin(), symbols.end()),
    length(symbols.size()) {}

size_t SymbolString::size() const {
    return length;
}

symbol SymbolString::operator[](size_t i) const {
    return symbols[i];
}

symbol SymbolString::first() const {
    return length == 0 ? nullptr : symbols[0];
}

symbol SymbolString::last() const {
    return length == 0 ? nullptr : symbols[length - 1];
}

std::strong_ordering SymbolString::operator<=>(const SymbolString &other) const {
    auto comp1 = size() <=> other.size();
    if (comp1 != std::strong_ordering::equal) {
        return comp1;
    }
    for (int i = 0; i < length; i++) {
        auto comp2 = (*this)[i]->id <=> other[i]->id;
        if (comp2 != std::strong_ordering::equal) {
            return comp2;
        }
    }
    return std::strong_ordering::equal;
}

SymbolString SymbolString::operator()(size_t start) const {
    return (*this)(start, length);
}

SymbolString SymbolString::operator()(size_t start, size_t end) const {
    return {{begin() + static_cast<int>(start), begin() + static_cast<int>(end)}};
}

SymbolString SymbolString::operator+(const symbol& sym) const {
    std::vector<symbol> x = symbols;
    x.push_back(sym);
    return {x};
}

std::vector<symbol>::const_iterator SymbolString::begin() const {
    return symbols.cbegin();
}
std::vector<symbol>::const_iterator SymbolString::end() const {
    return symbols.cend();
}
