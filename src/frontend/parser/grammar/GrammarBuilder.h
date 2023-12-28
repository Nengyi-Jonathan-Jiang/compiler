#pragma once

#include <vector>


#include "util/Cache.h"

#include "common/Symbol.h"

#include "frontend/parser/symbolSet.h"
#include "frontend/parser/SymbolString.h"
#include "frontend/parser/ParseRule.h"
#include "frontend/parser/grammar/Grammar.h"

class GrammarBuilder {
    grammarData data;

    void buildGrammar();

    void augmentRules();
    void classifySymbols();
    void createStartsWith();
    void initializeSets();
    void createSets();

    Grammar getResult();

public:

    GrammarBuilder(std::vector<parseRule> rules, symbol startSymbol);

    static Grammar compile(std::vector<parseRule> rules, symbol startSymbol);
};