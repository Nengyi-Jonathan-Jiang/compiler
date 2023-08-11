#include "lexer.h"
#include "fsm/DFA.h"
#include "fsm/FSM.h"
#include "fsm/NFA.h"
#include "fsm/NFABuilder.h"
#include "fsm/regexParser.h"

#include <utility>
#include <iostream>

lexer::lex::lex(struct lexer &lexer, std::string text) : lexer(lexer), text(std::move(text)) {}

std::optional<token> lexer::lex::try_get_next() {
    using impl_::FSM;

    if (position == text.size()) {
        return {token::create_end_token(text.size())};
    }

    // Run the DFA until we reach an error state
    symbol longest_accepted_symbol;
    const size_t token_start = position;
    size_t token_end = position;
    FSM::state_t curr_state = 0;

    while (curr_state != FSM::ERROR_STATE && position < text.size()) {
        auto [next_state, accepted] = lexer.dfa(curr_state, text[position++]);
        if (accepted) {
            longest_accepted_symbol = accepted;
            token_end = position;
        }
        curr_state = next_state;
    }

    // On the try_get_next call, we start right at the last token's first
    position = token_end;

    // If we can't accept the text, skip a character
    if (!longest_accepted_symbol) {
        position = token_start + 1;
        return {};
    }

    return {{longest_accepted_symbol, text.substr(token_start, token_end - token_start), token_start, token_end}};
}

token lexer::lex::next() {
    while (true) {
        std::optional<token> t = try_get_next();
        if (t.has_value()) {
            return t.value();
        }
    }
}

lexer::lex lexer::lexer::lex(std::string text) {
    return {*this, std::move(text)};
}

std::vector<token> lexer::lexer::lex_all(std::string text) {
    std::vector<token> res;
    struct lex _lex = lex(std::move(text));
    while(true) {
        const token& t = _lex.next();
        if(t.is_end_token()) {
            break;
        }
        res.push_back(t);
    }
    return res;
}

namespace {
    using state_t = lexer::impl_::FSM::state_t;
    using table_t = lexer::impl_::NFA::table_t;
    using entry_t = lexer::impl_::NFA::entry_t;

    struct DFA_state_info {
        std::set<lexer::impl_::FSM::state_t> nfa_states;
        symbol accepted_symbol;

        DFA_state_info(const std::set<lexer::impl_::FSM::state_t> &states, symbol &accepts)
                : nfa_states(states), accepted_symbol(accepts) {}

        std::strong_ordering operator<=>(const DFA_state_info &other) const {
            return nfa_states <=> other.nfa_states;
        }
    };

    struct multi_accept_NFA : public lexer::impl_::NFA {

        std::map<state_t, symbol> accepting_states;

        multi_accept_NFA(table_t transitions, std::map<state_t, symbol> acceptingStates)
                : NFA(std::move(transitions)), accepting_states(std::move(acceptingStates)) {}
    };

    class NFAtoDFAConverter {
        multi_accept_NFA merged;
        lexer::impl_::DFA result;
        state_t next_dfa_state{1};
        std::map<state_t, DFA_state_info> state_info;
        std::map<DFA_state_info, state_t> corresponding_state;
        std::set<DFA_state_info> seen_state_info;

        explicit NFAtoDFAConverter(multi_accept_NFA merged) : merged(std::move(merged)) {}

        auto NFA_epsilon_closure(const std::set<state_t> &states) {
            std::set<state_t> closure{states.begin(), states.end()};
            symbol accepted_symbol;
            compute_closure(closure, [&](state_t s) -> std::set<state_t> {
                const symbol as = merged.accepting_states[s];
                if (as && (!accepted_symbol || accepted_symbol->id > as->id)) {
                    accepted_symbol = as;
                }
                std::map<char, std::set<state_t>> &transitions = merged.table.at(s);
                return transitions.contains('\0') ? transitions.at('\0') : std::set<state_t>{};
            });
            return DFA_state_info(closure, accepted_symbol);
        }

        void convert() {
            // Find the state corresponding to state 0 and push to edge
            auto initial_state = NFA_epsilon_closure({0});
            state_info.emplace(0, initial_state);
            corresponding_state.emplace(initial_state, 0);
            std::set<state_t> states{0};

            compute_closure(states, [&](const state_t dfa_state) -> std::set<state_t> {

                std::map<char, std::set<state_t>> merged_transitions;
                for (const state_t nfa_state: state_info.at(dfa_state).nfa_states) {
                    for (auto &[c, targets]: merged.table.at(nfa_state)) {
                        if (c != '\0') {
                            merged_transitions[c].insert(targets.begin(), targets.end());
                        }
                    }
                }

                std::set<state_t> created_states;
                for (auto &[c, targets]: merged_transitions) {
                    auto new_state_info = NFA_epsilon_closure(targets);

                    if (seen_state_info.contains(new_state_info)) {
                        result.table[dfa_state][c] = corresponding_state[new_state_info];
                        continue;
                    }

                    seen_state_info.emplace(new_state_info);
                    state_t new_state = next_dfa_state++;

                    created_states.emplace(new_state);
                    state_info.emplace(new_state, new_state_info);
                    corresponding_state.emplace(new_state_info, new_state);
                    result.table[dfa_state][c] = new_state;
                    result.accepting_states.emplace(new_state, new_state_info.accepted_symbol);
                    if (!result.table.contains(new_state)) {
                        result.table.emplace(new_state, std::map<char, state_t>{});
                    }
                }
                return created_states;
            }, true);
        }

    public:
        static lexer::impl_::DFA convert(const multi_accept_NFA &merged) {
            NFAtoDFAConverter convertor{merged};
            convertor.convert();
            return convertor.result;
        }
    };


    std::tuple<table_t, state_t, state_t> remap_states(const table_t &table, state_t &state_counter) {
        const state_t mapped_start = state_counter++;
        const state_t mapped_end = state_counter++;
        std::map<state_t, state_t> mapped_states{{0, mapped_start},
                                                 {1, mapped_end}};

        for (auto [state, transitions]: table) {
            if (!mapped_states.contains(state)) {
                mapped_states[state] = state_counter++;
            }
            for (auto [c, targets]: transitions) {
                for (const state_t target: targets) {
                    if (!mapped_states.contains(target)) {
                        mapped_states[target] = state_counter++;
                    }
                }
            }
        }

        lexer::impl_::NFA::table_t new_table{{mapped_end, {}}};
        for (auto [state, transitions]: table) {
            lexer::impl_::NFA::entry_t new_transitions;
            for (auto [c, targets]: transitions) {
                std::set<lexer::impl_::FSM::state_t> new_targets;
                for (const lexer::impl_::FSM::state_t target: targets) {
                    new_targets.emplace(mapped_states[target]);
                }

                new_transitions.emplace(c, new_targets);
            }
            new_table.emplace(mapped_states[state], new_transitions);
        }

        return {new_table, mapped_start, mapped_end};
    }
}

lexer::lexer lexer::lexer::compile(const std::vector<lexRule> &rules) {

    multi_accept_NFA merged{{},
                            {}};
    impl_::FSM::state_t new_state_counter = 1;

    for (const auto &rule: rules) {
        auto parsed = impl_::RegexParser::parse(rule.regex);
        const impl_::NFA nfa = impl_::NFABuilder::NFAFromRegexParse(parsed);

        auto [table, mapped_start, mapped_end] = remap_states(nfa.table, new_state_counter);

        merged.table.insert(table.begin(), table.end());
        merged.table[0]['\0'].emplace(mapped_start);
        merged.accepting_states.emplace(mapped_end, rule.sym);
    }

    const impl_::DFA dfa = NFAtoDFAConverter::convert(merged);

    return {dfa};
}

lexer::lexer lexer::lexer::load(impl_::DFA::table_t transitions, impl_::DFA::accept_table_t accepts) {
    return {{std::move(transitions), std::move(accepts)}};
}