#include "NFABuilder.h"

// Disable clang recursion warning
#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

typedef RegexParser::tree_t tree_t;
typedef RegexParser::node_t node_t;
typedef NFA::table_t table_t;
typedef NFA::state_t state_t;
typedef NFA::entry_t entry_t;

[[nodiscard]] static NFA::table_t build_table(const node_t& node);

NFA NFABuilder::NFAFromRegexParse(const syntax_tree::AbstractSyntaxTree<regexNodeType>& parsedRegex) {
    return NFA{build_table(parsedRegex.root)};
}

[[nodiscard]] static table_t create_chars(const std::set<char>&);
[[nodiscard]] static table_t create_star(const node_t&);
[[nodiscard]] static table_t create_optional(const node_t&);
[[nodiscard]] static table_t create_alteration(const std::vector<node_t>&);
[[nodiscard]] static table_t create_string(const std::vector<node_t>& children);
[[nodiscard]] static table_t relink_table(table_t table, state_t initial, state_t final);

[[nodiscard]] static table_t build_table(const node_t& node) {
    switch(node->type){
        case regexNodeType::CHARS:
            return create_chars(tree_t::castToLeafNode(node)->get_value<std::set<char>>());

        case regexNodeType::ALTERNATION:
            return create_alteration(tree_t::castToInternalNode(node)->children);

        case regexNodeType::STAR:
            return create_star(tree_t::castToInternalNode(node)->children[NFA::INITIAL_STATE]);

        case regexNodeType::OPTION:
            return create_optional(tree_t::castToInternalNode(node)->children[NFA::INITIAL_STATE]);

        case regexNodeType::STRING:
            return create_string(tree_t::castToInternalNode(node)->children);
    }
    panic
}

[[nodiscard]] static table_t create_chars(const std::set<char>& chars){
    entry_t transitions;
    for(char c : chars) transitions[c] = {NFA::FINAL_STATE};
    return {{NFA::INITIAL_STATE, transitions}};
}

[[nodiscard]] static table_t create_alteration(const std::vector<node_t>& children) {
    table_t result;

    for(const node_t& child : children) {
        state_t intermediate_initial_state = NFA::nextUnusedState();
        table_t table = relink_table(build_table(child), intermediate_initial_state, NFA::FINAL_STATE);
        result.insert(table.begin(), table.end());
        result[NFA::INITIAL_STATE]['\0'].emplace(intermediate_initial_state);
    }
    return result;
}

[[nodiscard]] static table_t create_star(const node_t& child) {
    table_t table = build_table(child);
    state_t f_state = NFA::nextUnusedState();
    table = relink_table(table, NFA::INITIAL_STATE, f_state);
    table[NFA::INITIAL_STATE]['\0'].emplace(f_state);
    table[f_state]['\0'] = {NFA::INITIAL_STATE, NFA::FINAL_STATE};
    return table;
}

[[nodiscard]] static NFA::table_t create_optional(const node_t& child) {
    table_t table = build_table(child);
    table[NFA::INITIAL_STATE]['\0'].emplace(NFA::FINAL_STATE);
    return table;
}

[[nodiscard]] static NFA::table_t create_string(const std::vector<node_t>& children) {
    table_t result;
    for(int i = 0; i < children.size(); i++) {
        const node_t& child = children[i];
        state_t new_begin = i == 0 ? NFA::INITIAL_STATE : NFA::nextUnusedState();
        result = relink_table(result, NFA::INITIAL_STATE, new_begin);
        table_t table = relink_table(build_table(child), new_begin, NFA::FINAL_STATE);
        result.insert(table.begin(), table.end());
    }
    return result;
}

[[nodiscard]] static NFA::table_t relink_table(table_t table, state_t initial, state_t final) {
    // re-link every transition to accept (a.k.a. NFA::FINAL_STATE) to <final>
    for(auto& [_, transitions] : table) {
        for(auto& [__, targets] : transitions) {
            if(targets.contains(NFA::INITIAL_STATE) && initial != NFA::INITIAL_STATE) {
                targets.erase(NFA::INITIAL_STATE);
                targets.insert(initial);
            }
            if(targets.contains(NFA::FINAL_STATE) && final != NFA::FINAL_STATE) {
                targets.erase(NFA::FINAL_STATE);
                targets.insert(final);
            }
        }
    }
    if(initial != NFA::INITIAL_STATE) {
        table[initial] = table[NFA::INITIAL_STATE];
        table.erase(NFA::INITIAL_STATE);
    }
    return table;
}

#pragma clang diagnostic pop