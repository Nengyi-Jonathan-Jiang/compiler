#include "symbolSet.h"

symbolSet::symbolSet(std::initializer_list<symbol> symbols) : symbols{symbols.begin(), symbols.end()} {}

std::set<symbol>::const_iterator symbolSet::begin() const {
    return symbols.cbegin();
}

std::set<symbol>::const_iterator symbolSet::end() const {
    return symbols.cend();
}

bool symbolSet::operator+=(symbol sym) {
    return symbols.insert(std::move(sym)).second;
}

bool symbolSet::operator+=(symbolSet sym) {
    bool res = false;
    for (const symbol &s: sym) {
        res |= symbols.insert(s).second;
    }
    return res;
}

bool symbolSet::operator+=(SymbolString sym) {
    bool res = false;
    for (const symbol &s: sym) {
        res |= symbols.insert(s).second;
    }
    return res;
}

bool symbolSet::operator+=(std::initializer_list<symbol> sym) {
    bool res = false;
    for (const symbol &s: sym) {
        res |= symbols.insert(s).second;
    }
    return res;
}

bool symbolSet::operator-=(const symbol &sym) {
    return symbols.erase(sym) > 0;
}

bool symbolSet::operator-=(symbolSet sym) {
    bool res = false;
    for (const symbol &s: sym) {
        res |= symbols.erase(s) > 0;
    }
    return res;
}

bool symbolSet::operator-=(SymbolString sym) {
    bool res = false;
    for (const symbol &s: sym) {
        res |= symbols.erase(s) > 0;
    }
    return res;
}

bool symbolSet::operator-=(std::initializer_list<symbol> sym) {
    bool res = false;
    for (const symbol &s: sym) {
        res |= symbols.erase(s) > 0;
    }
    return res;
}

bool symbolSet::contains(const symbol &sym) const {
    return symbols.contains(sym);
}