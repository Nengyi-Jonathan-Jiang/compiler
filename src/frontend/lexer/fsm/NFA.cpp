#include "NFA.h"
#include <iostream>

using state_t = FSM::state_t;

NFA::state_t NFA::nextUnusedState() {
    static NFA::state_t next = 2;
    return next++;
}

void NFA::print_self() const {
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
    for (char c: all_chars) {
        std::cout << c << "\t | ";
    }
    std::cout << "null table:\n";

    for (const auto &[state, transitions]: table) {
        if (state == 1) {
            continue;
        }

        std::cout << "-----+";
        for (int i = 0; i < all_chars.size(); i++) {
            std::cout << "-------+";
        }
        std::cout << "-------------------\n";

        std::cout << (state == 0 ? "i" : std::to_string(state)) << "\t" << " | ";

        for (const char c: all_chars) {
            if (transitions.contains(c)) {
                for (const state_t to: transitions.at(c)) {
                    std::cout << (to == 0 ? "i" : to == 1 ? "a" : std::to_string(to));
                }
            } else {
                std::cout << " ";
            }
            std::cout << "\t | ";
        }

        if (transitions.contains('\0')) {
            for (const state_t to: transitions.at('\0')) {
                std::cout << (to == 0 ? "i" : to == 1 ? "a" : std::to_string(to)) << ", ";
            }
        }

        std::cout << "\n";
    }
    std::cout << std::endl;
}
