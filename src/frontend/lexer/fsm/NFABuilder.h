#pragma once

#include "NFA.h"
#include "regexParser.h"

namespace lexer::impl_ {
    class NFABuilder {
    public:
        static NFA NFAFromRegexParse(const syntax_tree::AbstractSyntaxTree<regexNodeType>& parsedRegex);
    };
}