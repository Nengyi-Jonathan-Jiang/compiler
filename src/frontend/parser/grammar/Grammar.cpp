#include "Grammar.h"

#include <utility>
#include <iostream>

Grammar::Grammar(grammarData data) : data(std::move(data)) {}

symbolSet Grammar::first(const symbolString &symbols) {
    // First set of empty token symbols is {epsilon}
    if (symbols.size() == 0) {
        return {nullptr};
    }

    return followCache.computeIfAbsent(symbols, [this](const symbolString &s) -> symbolSet {
        symbolSet res{first(s.first())};

        if (isNullable(s.first())) {
            res += first(s(1));
        }

        return res;
    });
}

symbolSet Grammar::follow(const symbolString &symbols) {
    // Follow set of empty token symbols is {epsilon}
    if (symbols.size() == 0) {
        return {nullptr};
    }

    return followCache.computeIfAbsent(symbols, [this](const symbolString &s) -> symbolSet {
        symbolSet res{follow(s.last())};

        if (isNullable(s.last())) {
            res += follow(s(0, s.size() - 1));
        }

        return res;
    });
}

std::vector<parseRule> Grammar::productionsForSymbol(const symbol& s) const {
    return data.productionsForSymbol.at(s);
}

const symbolSet &Grammar::allSymbols() const {
    return data.allSymbols;
}
const symbolSet &Grammar::terminals() const {
    return data.terminals;
}
const symbolSet &Grammar::nonTerminals() const {
    return data.nonTerminals;
}
const symbolSet &Grammar::nullableSet() const {
    return data.nullableSet;
}

bool Grammar::isTerminal(const symbol& s) const {
    return data.terminals.contains(s);
}
bool Grammar::isNonTerminal(const symbol& s) const {
    return data.nonTerminals.contains(s);
}
bool Grammar::isNullable(const symbol& s) const {
    return data.nullableSet.contains(s);
}

const symbolSet& Grammar::first(const symbol& s) const {
    return data.firstSets.at(s);
}
const symbolSet& Grammar::follow(const symbol &s) const {
    return data.followSets.at(s);
}

void Grammar::print_info() const {
    std::cout << "Terminals: ";
    for(const auto& t : terminals()) {
        std::cout << t->name << " ";
    }
    std::cout << "\nNon-terminals: ";
    for(const auto& t : nonTerminals()) {
        std::cout << t->name << " ";
    }
    std::cout << "\nNullable symbols: ";
    for(const auto& t : nullableSet()) {
        std::cout << t->name << " ";
    }
    std::cout << "\nFIRST sets:\n";
    for(const auto& t : allSymbols()) {
        std::cout << t->name << ": ";
        for(const auto& t2 : first(t)){
            std::cout << (t2 ? t2->name : "NULL") << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\nFOLLOW sets:\n";
    for(const auto& t : allSymbols()) {
        std::cout << t->name << ": ";
        for(const auto& t2 : follow(t)){
            std::cout << (t2 ? t2->name : "NULL") << " ";
        }
        std::cout << "\n";
    }
}
