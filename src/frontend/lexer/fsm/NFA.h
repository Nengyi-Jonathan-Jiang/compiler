#pragma once

#include <map>
#include <set>
#include "../../../util.h"
#include "../../../common/syntax_tree.h"
#include "FSM.h"

/// State 0 = start, State 1 = accept
/// Condition: final state may not have outgoing transition
struct NFA : public FSM {
    static constexpr state_t INITIAL_STATE = 0;
    static constexpr state_t FINAL_STATE = 1;

    table_t table;

    explicit NFA(table_t transitions) : table(std::move(transitions)) {}

    static state_t nextUnusedState();

    void print_self() const;
};
