#pragma once

#include <string>
#include <set>
#include "../../common/Symbol.h"
#include "SymbolString.h"

class symbolSet {
private:
    bool _needsUpdateString = true;
    std::string str;
    std::set<symbol> symbols;

public:
    symbolSet() = default;

    symbolSet(std::initializer_list<symbol> symbols);

    [[nodiscard]] std::set<symbol>::const_iterator begin() const;
    [[nodiscard]] std::set<symbol>::const_iterator end() const;

    bool operator+=(symbol sym);
    bool operator+=(symbolSet sym);
    bool operator+=(SymbolString sym);
    bool operator+=(std::initializer_list<symbol> sym);

    bool operator-=(const symbol &sym);
    bool operator-=(symbolSet sym);
    bool operator-=(SymbolString sym);
    bool operator-=(std::initializer_list<symbol> sym);

    std::strong_ordering operator<=>(const symbolSet& other) const {
        return symbols <=> other.symbols;
    }

    [[nodiscard]] bool contains(const symbol &sym) const;
};