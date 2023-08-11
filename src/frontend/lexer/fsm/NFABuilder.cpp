#include "NFABuilder.h"
#include "NFA.h"
#include "regexParser.h"

// Disable clang recursion warning
#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

using tree_t = lexer::impl_::RegexParser::tree_t;
using node_t = lexer::impl_::RegexParser::node_t;
using table_t = lexer::impl_::NFA::table_t;
using state_t = lexer::impl_::NFA::state_t;
using entry_t = lexer::impl_::NFA::entry_t;

[[nodiscard]] static lexer::impl_::NFA::table_t build_table(const node_t& node);

lexer::impl_::NFA lexer::impl_::NFABuilder::NFAFromRegexParse(const syntax_tree::AbstractSyntaxTree<regexNodeType>& parsedRegex) {
    return lexer::impl_::NFA{build_table(parsedRegex.root)};
}

[[nodiscard]] static table_t create_chars(const std::set<char>&);
[[nodiscard]] static table_t create_star(const node_t&);
[[nodiscard]] static table_t create_optional(const node_t&);
[[nodiscard]] static table_t create_alteration(const std::vector<node_t>&);
[[nodiscard]] static table_t create_string(const std::vector<node_t>& children);
[[nodiscard]] static table_t relink_table(table_t table, state_t initial, state_t final);

[[nodiscard]] static table_t build_table(const node_t& node) {
    switch(node->type){
        case lexer::impl_::regexNodeType::CHARS:
            return create_chars(tree_t::castToLeafNode(node)->get_value<std::set<char>>());

        case lexer::impl_::regexNodeType::ALTERNATION:
            return create_alteration(tree_t::castToInternalNode(node)->children);

        case lexer::impl_::regexNodeType::STAR:
            return create_star(tree_t::castToInternalNode(node)->children[lexer::impl_::NFA::INITIAL_STATE]);

        case lexer::impl_::regexNodeType::OPTION:
            return create_optional(tree_t::castToInternalNode(node)->children[lexer::impl_::NFA::INITIAL_STATE]);

        case lexer::impl_::regexNodeType::STRING:
            return create_string(tree_t::castToInternalNode(node)->children);
    }
    panic
}

[[nodiscard]] static table_t create_chars(const std::set<char>& chars){
    entry_t transitions;
    for(char c : chars) transitions[c] = {lexer::impl_::NFA::FINAL_STATE};
    return {{lexer::impl_::NFA::INITIAL_STATE, transitions}};
}

[[nodiscard]] static table_t create_alteration(const std::vector<node_t>& children) {
    table_t result;

    for(const node_t& child : children) {
        state_t intermediate_initial_state = lexer::impl_::NFA::nextUnusedState();
        table_t table = relink_table(build_table(child), intermediate_initial_state, lexer::impl_::NFA::FINAL_STATE);
        result.insert(table.begin(), table.end());
        result[lexer::impl_::NFA::INITIAL_STATE]['\0'].emplace(intermediate_initial_state);
    }
    return result;
}

[[nodiscard]] static table_t create_star(const node_t& child) {
    table_t table = build_table(child);
    state_t f_state = lexer::impl_::NFA::nextUnusedState();
    table = relink_table(table, lexer::impl_::NFA::INITIAL_STATE, f_state);
    table[lexer::impl_::NFA::INITIAL_STATE]['\0'].emplace(f_state);
    table[f_state]['\0'] = {lexer::impl_::NFA::INITIAL_STATE, lexer::impl_::NFA::FINAL_STATE};
    return table;
}

[[nodiscard]] static lexer::impl_::NFA::table_t create_optional(const node_t& child) {
    table_t table = build_table(child);
    table[lexer::impl_::NFA::INITIAL_STATE]['\0'].emplace(lexer::impl_::NFA::FINAL_STATE);
    return table;
}

[[nodiscard]] static lexer::impl_::NFA::table_t create_string(const std::vector<node_t>& children) {
    table_t result;
    for(int i = 0; i < children.size(); i++) {
        const node_t& child = children[i];
        state_t new_begin = i == 0 ? lexer::impl_::NFA::INITIAL_STATE : lexer::impl_::NFA::nextUnusedState();
        result = relink_table(result, lexer::impl_::NFA::INITIAL_STATE, new_begin);
        table_t table = relink_table(build_table(child), new_begin, lexer::impl_::NFA::FINAL_STATE);
        result.insert(table.begin(), table.end());
    }
    return result;
}

[[nodiscard]] static lexer::impl_::NFA::table_t relink_table(table_t table, state_t initial, state_t final) {
    // re-link every transition to accept (a.k.a. NFA::FINAL_STATE) to <final>
    for(auto& [_, transitions] : table) {
        for(auto& [__, targets] : transitions) {
            if(targets.contains(lexer::impl_::NFA::INITIAL_STATE) && initial != lexer::impl_::NFA::INITIAL_STATE) {
                targets.erase(lexer::impl_::NFA::INITIAL_STATE);
                targets.insert(initial);
            }
            if(targets.contains(lexer::impl_::NFA::FINAL_STATE) && final != lexer::impl_::NFA::FINAL_STATE) {
                targets.erase(lexer::impl_::NFA::FINAL_STATE);
                targets.insert(final);
            }
        }
    }
    if(initial != lexer::impl_::NFA::INITIAL_STATE) {
        table[initial] = table[lexer::impl_::NFA::INITIAL_STATE];
        table.erase(lexer::impl_::NFA::INITIAL_STATE);
    }
    return table;
}

#pragma clang diagnostic pop