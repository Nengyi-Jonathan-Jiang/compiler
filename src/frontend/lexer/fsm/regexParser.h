#pragma once

#include <map>
#include <set>
#include <stack>
#include "../../../util/util.h"
#include "../../../common/syntax_tree.h"
#include "DFA.h"

namespace lexer::impl_ {
    enum class regexNodeType { CHARS, ALTERNATION, STAR, OPTION, STRING };
    enum class regexOperatorType { STRING, ALTERNATION, PAREN };

    class RegexParser {
    public:
        using tree_t = syntax_tree::AbstractSyntaxTree<regexNodeType>;
        using node_t = syntax_tree::ASTNode<regexNodeType>;
        using internal_node_t = syntax_tree::ASTInternalNode<regexNodeType>;
        using leaf_node_t = syntax_tree::ASTLeafNode<regexNodeType>;

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
}

std::ostream& operator<<(std::ostream& os, lexer::impl_::regexNodeType t);

std::ostream& operator<<(std::ostream& os, lexer::impl_::RegexParser::node_t node);