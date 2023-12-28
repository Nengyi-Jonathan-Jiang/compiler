#pragma once

#include <map>
#include <set>
#include <utility>
#include "../../../util/util.h"
#include "FSM.h"
#include "../../../common/Symbol.h"

namespace lexer::impl_ {
    struct DFA : public FSM {
        using entry_t = std::map<char, state_t>;
        using table_t = std::map<state_t, entry_t>;
        using accept_table_t = std::map<state_t, symbol>;

        table_t table;
        accept_table_t accepting_states;

        DFA() = default;

        DFA(table_t transitions, accept_table_t acceptingStates) : table(std::move(transitions)),
                                                                   accepting_states(std::move(acceptingStates)) {}

        void print_self() const;

        std::pair<state_t, symbol> operator()(state_t t, char c);
    };
}