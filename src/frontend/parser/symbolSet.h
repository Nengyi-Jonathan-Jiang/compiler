#pragma once

#include <string>
#include <set>
#include "../../common/symbol.h"
#include "symbolString.h"

class symbolSet {
private:
    bool _needsUpdateString = true;
    std::string str;
    std::set<symbol> symbols;

public:
    symbolSet() = default;

    symbolSet(std::initializer_list<symbol> symbols);

    std::set<symbol>::const_iterator begin() const;
    std::set<symbol>::const_iterator end() const;

    bool operator+=(symbol sym);
    bool operator+=(symbolSet sym);
    bool operator+=(symbolString sym);
    bool operator+=(std::initializer_list<symbol> sym);

    bool operator-=(const symbol &sym);
    bool operator-=(symbolSet sym);
    bool operator-=(symbolString sym);
    bool operator-=(std::initializer_list<symbol> sym);

    std::strong_ordering operator<=>(const symbolSet& other) const {
        return symbols <=> other.symbols;
    }

    [[nodiscard]] bool contains(const symbol &sym) const;
};