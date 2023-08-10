#include <iostream>
#include "DFA.h"
#include "FSM.h"

void DFA::print_self() const {
    std::set<state_t> all_states;
    std::set<char> all_chars;
    for (const auto &[state, transitions]: table) {
        all_states.emplace(state);
        for (const auto &[c, _]: transitions) {
            if (c != '\0') {
                all_chars.emplace(c);
            }
        }
    }

    std::cout << "\t | ";
    for (char const c: all_chars) {
        std::cout << c << "\t | ";
    }
    std::cout << "accepts\n";

    for (const auto &[state, transitions]: table) {
        std::cout << "-----+";
        for (int i = 0; i < all_chars.size(); i++) {
            std::cout << "-------+";
        }
        std::cout << "---------------\n";

        std::cout << (state == 0 ? "i" : std::to_string(state)) << "\t" << " | ";

        for (char const c: all_chars) {
            if (transitions.contains(c)) {
                state_t const to = transitions.at(c);
                std::cout << (to == 0 ? "i" : std::to_string(to));
            } else {
                std::cout << " ";
            }
            std::cout << "\t | ";
        }

        if (accepting_states.contains(state)) {
            const symbol accepted = accepting_states.at(state);
            if (accepted) {
                std::cout << accepted->name;
            }
        }

        std::cout << "\n";
    }
    std::cout << '\n';
}

std::pair<FSM::state_t, symbol> DFA::operator()(FSM::state_t t, char c) {
    if (!table[t].contains(c)) {
        return make_pair(ERROR_STATE, symbol{});
    }

    const state_t next = table[t][c];
    return make_pair(next, accepting_states[next]);
}
