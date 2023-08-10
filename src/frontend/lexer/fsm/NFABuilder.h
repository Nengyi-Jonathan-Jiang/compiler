#pragma once

#include "NFA.h"
#include "regexParser.h"

class NFABuilder {
public:
    static NFA NFAFromRegexParse(const syntax_tree::AbstractSyntaxTree<regexNodeType>& parsedRegex);
};