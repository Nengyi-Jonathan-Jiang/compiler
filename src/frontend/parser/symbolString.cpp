#include "symbolString.h"

#include <utility>


symbolString::symbolString(const std::vector<symbol>& symbols) : length(symbols.size()), symbols(symbols) {}

symbolString::symbolString(std::initializer_list<symbol> symbols) :
    symbols(symbols.begin(), symbols.end()),
    length(symbols.size()) {}

size_t symbolString::size() const {
    return length;
}

symbol symbolString::operator[](size_t i) const {
    return symbols[i];
}

symbol symbolString::first() const {
    return length == 0 ? nullptr : symbols[0];
}

symbol symbolString::last() const {
    return length == 0 ? nullptr : symbols[length - 1];
}

std::strong_ordering symbolString::operator<=>(const symbolString &other) const {
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

symbolString symbolString::operator()(size_t start) const {
    return (*this)(start, length);
}

symbolString symbolString::operator()(size_t start, size_t end) const {
    return {{begin() + static_cast<int>(start), begin() + static_cast<int>(end)}};
}

symbolString symbolString::operator+(const symbol& sym) const {
    std::vector<symbol> x = symbols;
    x.push_back(sym);
    return {x};
}

std::vector<symbol>::const_iterator symbolString::begin() const {
    return symbols.cbegin();
}
std::vector<symbol>::const_iterator symbolString::end() const {
    return symbols.cend();
}
