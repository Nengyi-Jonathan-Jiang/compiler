#pragma once

#include <map>
#include <set>
#include <stack>
#include "../../../util.h"
#include "../../../common/syntax_tree.h"
#include "DFA.h"

enum class regexNodeType { CHARS, ALTERNATION, STAR, OPTION, STRING };
enum class regexOperatorType { STRING, ALTERNATION, PAREN };

std::ostream& operator<<(std::ostream& os, regexNodeType t);

class RegexParser {
public:
    typedef syntax_tree::AbstractSyntaxTree<regexNodeType> tree_t;
    typedef syntax_tree::ASTNode<regexNodeType> node_t;
    typedef syntax_tree::ASTInternalNode<regexNodeType> internal_node_t;
    typedef syntax_tree::ASTLeafNode<regexNodeType> leaf_node_t;

private:
    RegexParser() = default;
    std::stack<node_t> stack {};
    std::stack<regexOperatorType> operator_stack;

    bool hasOperatorsLeft();
    node_t stackPop();
    regexOperatorType operatorStackPop();
    regexOperatorType operatorStackPeek();

    static node_t append_element(const node_t &x, const node_t &y);

    void chars(const std::set<char> &chars);
    void concat();
    void alternation();
    void begin_group();
    void end_group();
    void apply(regexNodeType op);

    tree_t result();

public:
    static tree_t parse(const std::string& regex);
};

std::ostream& operator<<(std::ostream& os, RegexParser::node_t node);