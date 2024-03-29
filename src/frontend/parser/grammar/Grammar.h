#pragma once

#include <utility>
#include <vector>
#include "../ParseRule.h"
#include "../symbolSet.h"
#include "../../../util/Cache.h"

struct grammarData {
    std::vector<parseRule> rules;
    parseRule start;

    std::map<symbol, std::vector<parseRule>> productionsForSymbol;
    symbolSet allSymbols, terminals, nonTerminals, nullableSet;
    std::map<symbol, symbolSet> firstSets, followSets;

    grammarData()=default;
};

class Grammar {
    grammarData data;
    Cache<SymbolString, symbolSet> firstCache, followCache;

public:
    Grammar(grammarData data);

    [[nodiscard]] std::vector<parseRule> productionsForSymbol(const symbol& s) const;
    [[nodiscard]] const symbolSet& allSymbols() const;
    [[nodiscard]] const symbolSet& terminals() const;
    [[nodiscard]] const symbolSet& nonTerminals() const;
    [[nodiscard]] const symbolSet& nullableSet() const;

    [[nodiscard]] bool isTerminal(const symbol& s) const;
    [[nodiscard]] bool isNonTerminal(const symbol& s) const;
    [[nodiscard]] bool isNullable(const symbol& s) const;

    [[nodiscard]] const symbolSet& first(const symbol& s) const;
    [[nodiscard]] const symbolSet& follow(const symbol& s) const;

    [[nodiscard]] symbolSet first(const SymbolString& symbols);
    [[nodiscard]] symbolSet follow(const SymbolString &symbols);

    [[nodiscard]] const grammarData& getData() const;

    void print_info() const;
};
