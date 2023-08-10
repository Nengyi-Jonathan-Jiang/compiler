#pragma once

#include <map>
#include <set>
#include "../../../util.h"
#include "FSM.h"
#include "../../../common/symbol.h"

struct DFA : public FSM {
    using entry_t = std::map<char, state_t>;

    std::map<state_t, entry_t> table;
    std::map<state_t, symbol> accepting_states;

    DFA() = default;

    DFA(const std::map<state_t, entry_t> &transitions, const std::map<state_t, symbol> &acceptingStates)
            : table(transitions), accepting_states(acceptingStates) {}

    void print_self() const;

    std::pair<state_t, symbol> operator()(state_t t, char c);
};