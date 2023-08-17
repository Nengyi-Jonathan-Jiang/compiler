#pragma once

#include <vector>


#include "util/cache.h"

#include "common/symbol.h"

#include "frontend/parser/symbolSet.h"
#include "frontend/parser/symbolString.h"
#include "frontend/parser/parseRule.h"
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