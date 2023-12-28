#pragma once

#include <utility>
#include <string>
#include <vector>
#include <functional>

#include "../common/token.h"

#include "../../common/Symbol.h"
#include "../../common/algorithms.h"
#include "fsm/FSM.h"
#include "fsm/DFA.h"
#include "fsm/regexParser.h"
#include "fsm/NFABuilder.h"

namespace lexer {
    struct lex;
    struct lexRule;

    class lexer {
    private:
    public:
        impl_::DFA dfa;

        lexer(impl_::DFA dfa) : dfa(std::move(dfa)) {}
        friend class lex;

    public:
        lex lex(std::string text);
        std::vector<token> lex_all(std::string text);

        [[nodiscard]] static lexer load(impl_::DFA::table_t transitions, impl_::DFA::accept_table_t accepts);
        [[nodiscard]] static lexer compile(const std::vector<lexRule>& rules);
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