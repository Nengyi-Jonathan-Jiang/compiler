#pragma once

#include <map>
#include <set>
#include "../../../util/util.h"
#include "../../../common/syntax_tree.h"
#include "FSM.h"

namespace lexer::impl_ {
    /// State 0 = start, State 1 = accept
    /// Condition: final state may not have outgoing transition
    struct NFA : public lexer::impl_::FSM {
        static constexpr state_t INITIAL_STATE = 0;
        static constexpr state_t FINAL_STATE = 1;

        table_t table;

        explicit NFA(table_t transitions) : table(std::move(transitions)) {}

        static state_t nextUnusedState();

        void print_self() const;
    };
}
