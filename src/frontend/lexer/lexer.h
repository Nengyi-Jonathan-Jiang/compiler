#pragma once

#include <utility>
#include <string>
#include <vector>

#include "lexerBuilder.h"
#include "../../frontend/common/token.h"

#include "fsm/DFA.h"
#include "../../common/symbol.h"
#include "fsm/FSM.h"
#include "fsm/NFA.h"
#include "fsm/regexParser.h"
#include "fsm/NFABuilder.h"
#include "../../common/algorithms.h"

namespace lexer {
    struct lex;
    class lexerBuilder;
    struct lexRule;

    class lexer {
    private:
        DFA dfa;

        lexer(DFA dfa) : dfa(std::move(dfa)) {}

        friend class lexerBuilder;
        friend class lex;

    public:
        lex lex(std::string text);
        std::vector<token> lex_all(std::string text);

        static lexer compile(const std::vector<lexRule>& rules);
    };

    struct lex {
    private:
        lexer lexer;
        std::string text;
        size_t position {0};
        lex(struct lexer& lexer, std::string text);
        friend class lexer;
    public:
        std::optional<token> try_get_next();
        token next();
    };

    struct lexRule {
        symbol sym;
        std::string regex;

        lexRule(symbol s, std::string regex) : sym(std::move(s)), regex(std::move(regex)) {}
    };
}

