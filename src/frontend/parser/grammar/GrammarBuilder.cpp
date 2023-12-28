#include "GrammarBuilder.h"

#include <utility>

void GrammarBuilder::buildGrammar() {
    augmentRules();
    classifySymbols();
    createStartsWith();
    initializeSets();
    createSets();
}

GrammarBuilder::GrammarBuilder(std::vector<parseRule> rules, symbol startSymbol) {
    data.rules = (std::move(rules));
    data.start = std::make_shared<parseRule_>(symbol_::START, std::vector<symbol>{{std::move(startSymbol)}});
}

void GrammarBuilder::augmentRules() { data.rules.push_back(data.start); }

void GrammarBuilder::classifySymbols() {
    data.allSymbols += {symbol_::START, symbol_::END};
    data.nonTerminals += symbol_::START;

    for (auto &rule: data.rules) {
        data.nonTerminals += rule->getLhs();
        data.allSymbols += rule->getRhs();
        data.allSymbols += rule->getRhs();
    }

    data.terminals += data.allSymbols;
    data.terminals -= data.nonTerminals;
}

void GrammarBuilder::createStartsWith() {
    for (symbol sym: data.allSymbols) {
        data.productionsForSymbol.emplace(sym, std::vector<parseRule>{});
    }
    for (const parseRule &rule: data.rules) {
        data.productionsForSymbol[rule->getLhs()].push_back(rule);
    }
}

void GrammarBuilder::initializeSets() {
    for (symbol sym: data.allSymbols) {
        data.firstSets.emplace(sym, symbolSet{});
        data.followSets.emplace(sym, symbolSet{});
        if (data.terminals.contains(sym)) {
            data.firstSets[sym] += sym;
        }
    }

    data.followSets[symbol_::START] += symbol_::END;
}

void GrammarBuilder::createSets() {
    for (bool updated = true; updated;) {
        updated = false;
        for (const parseRule &rule: data.rules) {
            const symbol lhs = rule->getLhs();
            const SymbolString rhs = rule->getRhs();
            for (const symbol &sym: rhs) {
                updated |= data.firstSets[lhs] += data.firstSets[sym];
                if (!data.nullableSet.contains(sym)) {
                    goto brk;
                }
            }
            updated |= data.nullableSet += lhs;
            brk:

            symbolSet aux = data.followSets[lhs];
            for (const symbol &s: rhs | std::ranges::views::reverse) {
                if (data.nonTerminals.contains(s)) {
                    updated |= data.followSets[s] += aux;
                }
                if (data.nullableSet.contains(s)) {
                    aux += data.firstSets[s];
                } else {
                    aux = data.firstSets[s];
                }
            }
        }
    }
}

Grammar GrammarBuilder::getResult() {
    return {data};
}

Grammar GrammarBuilder::compile(std::vector<parseRule> rules, symbol startSymbol) {
    GrammarBuilder builder(std::move(rules), std::move(startSymbol));
    builder.buildGrammar();
    return builder.getResult();
}
