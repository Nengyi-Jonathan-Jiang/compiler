#include "regexParser.h"
#include "FSM.h"
#include "DFA.h"

bool lexer::impl_::RegexParser::hasOperatorsLeft() {
    return !operator_stack.empty();
}

lexer::impl_::RegexParser::node_t lexer::impl_::RegexParser::stackPop() {
    auto res = stack.top();
    stack.pop();
    return res;
}

lexer::impl_::regexOperatorType lexer::impl_::RegexParser::operatorStackPeek() {
    return operator_stack.top();
}

lexer::impl_::RegexParser::node_t lexer::impl_::RegexParser::append_element(const lexer::impl_::RegexParser::node_t &x, const lexer::impl_::RegexParser::node_t &y) {
    tree_t::castToInternalNode(x)->add_child(y);
    return x;
}

void lexer::impl_::RegexParser::chars(const std::set<char> &chars) {
    stack.push(tree_t::createLeafNode(regexNodeType::CHARS, chars));
}

void lexer::impl_::RegexParser::concat() {
    while (hasOperatorsLeft() && operatorStackPeek() == regexOperatorType::STRING) {
        node_t x2 = stackPop();
        node_t x1 = stackPop();
        operatorStackPop();
        stack.push(append_element(x1, x2));
    }
    operator_stack.push(regexOperatorType::STRING);
}

void lexer::impl_::RegexParser::alternation() {
    while (hasOperatorsLeft() && operatorStackPeek() != regexOperatorType::PAREN) {
        node_t x2 = stackPop();
        node_t x1 = stackPop();
        switch (operatorStackPop()) {
            case regexOperatorType::ALTERNATION:
                if (x1->type != regexNodeType::ALTERNATION)
                    stack.push(tree_t::createInternalNode(regexNodeType::ALTERNATION, {x1, x2}));
                else stack.push(append_element(x1, x2));
                break;
            case regexOperatorType::STRING:
                stack.push(append_element(x1, x2));
                break;
            default: panic
        }
    }
    operator_stack.push(regexOperatorType::ALTERNATION);
    stack.push(tree_t::createInternalNode(regexNodeType::STRING, {}));
}

void lexer::impl_::RegexParser::begin_group() {
    operator_stack.push(regexOperatorType::PAREN);
    stack.push(tree_t::createInternalNode(regexNodeType::STRING, {}));
}

void lexer::impl_::RegexParser::end_group() {
    for (regexOperatorType t; (t = operatorStackPop()) != regexOperatorType::PAREN;) {
        node_t x2 = stackPop();
        node_t x1 = stackPop();
        switch (t) {
            case regexOperatorType::ALTERNATION:
                if (x1->type != regexNodeType::ALTERNATION)
                    stack.push(tree_t::createInternalNode(regexNodeType::ALTERNATION, {x1, x2}));
                else stack.push(append_element(x1, x2));
                break;
            case regexOperatorType::STRING:
                stack.push(append_element(x1, x2));
                break;
            default: panic
        }
    }
}

void lexer::impl_::RegexParser::apply(regexNodeType op) {
    stack.push(tree_t::createInternalNode(op, {stackPop()}));
}

syntax_tree::AbstractSyntaxTree<lexer::impl_::regexNodeType> lexer::impl_::RegexParser::result() {
    return syntax_tree::AbstractSyntaxTree<regexNodeType>{stack.top()};
}

syntax_tree::AbstractSyntaxTree<lexer::impl_::regexNodeType> lexer::impl_::RegexParser::parse(const std::string &regex) {
    RegexParser reParser;

    reParser.begin_group();
    for (int i = 0; i < regex.size(); i++) {
        char c = regex[i];
        switch (c) {
            case '|':
                reParser.alternation();
                break;
            case '(':
                reParser.concat();
                reParser.begin_group();
                break;
            case ')':
                reParser.end_group();
                break;
            case '*':
                reParser.apply(regexNodeType::STAR);
                break;
            case '?':
                reParser.apply(regexNodeType::OPTION);
                break;
            case '.':
                reParser.chars(lexer::impl_::DFA::all_chars);
                break;
            case '[': {
                reParser.concat();
                bool conjugate = false;
                std::set<char> chars;
                if (regex[i + 1] == '^') {
                    conjugate = true;
                    chars = lexer::impl_::FSM::all_chars;
                    i++;
                }
                while ((c = regex[++i]) != ']') {
                    if (c == '\\') {
                        switch (regex[++i]) {
                            case 'w':
                                for (const char ch: lexer::impl_::FSM::word_chars) {
                                    if (conjugate) {
                                        chars.erase(ch);
                                    } else {
                                        chars.emplace(ch);
                                    }
                                }
                                break;
                            case 'l':
                                for (const char ch: lexer::impl_::FSM::letters) {
                                    if (conjugate) {
                                        chars.erase(ch);
                                    } else {
                                        chars.emplace(ch);
                                    }
                                }
                                break;
                            case 'd':
                                for (const char ch: lexer::impl_::FSM::digits) {
                                    if (conjugate) {
                                        chars.erase(ch);
                                    } else {
                                        chars.emplace(ch);
                                    }
                                }
                                break;
                            default:
                                if (conjugate) {
                                    chars.erase(regex[i]);
                                } else {
                                    chars.emplace(regex[i]);
                                }
                        }
                    }
                    else if (conjugate) {
                        chars.erase(c);
                    } else {
                        chars.emplace(c);
                    }
                }
                if (chars.empty()) panic
                reParser.chars(chars);
                break;
            }
            case ']': panic
            case '\\':
                c = regex[++i];
                switch(c){
                    case 'w':
                        reParser.concat();
                        reParser.chars(lexer::impl_::FSM::word_chars);
                        break;
                    case 'l':
                        reParser.concat();
                        reParser.chars(lexer::impl_::FSM::letters);
                        break;
                    case 'd':
                        reParser.concat();
                        reParser.chars(lexer::impl_::FSM::digits);
                        break;
                    default:
                        reParser.concat();
                        reParser.chars({c});
                }
                break;
            default:
                reParser.concat();
                reParser.chars({c});
                break;
        }
    }
    reParser.end_group();
    return reParser.result();
}

lexer::impl_::regexOperatorType lexer::impl_::RegexParser::operatorStackPop() {
    auto res = operator_stack.top();
    operator_stack.pop();
    return res;
}

std::ostream &operator<<(std::ostream &os, lexer::impl_::regexNodeType t) {
    switch (t) {
        case lexer::impl_::regexNodeType::CHARS:
            return os << "chars";
        case lexer::impl_::regexNodeType::ALTERNATION:
            return os << "<|>";
        case lexer::impl_::regexNodeType::STAR:
            return os << "<*>";
        case lexer::impl_::regexNodeType::OPTION:
            return os << "<?>";
        case lexer::impl_::regexNodeType::STRING:
            return os << "<#>";
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, lexer::impl_::RegexParser::node_t node) {
    int indent_level = 0;
    std::stack<lexer::impl_::RegexParser::node_t> stk;
    stk.push(node);

    while (!stk.empty()) {
        auto n = stk.top();
        stk.pop();

        if (!n) {
            indent_level--;
            for (int i = 0; i < indent_level; i++) os << "    ";
            os << "}\n";
            continue;
        }

        for (int i = 0; i < indent_level; i++) os << "    ";
        os << n->type;

        if (n->isLeaf()) {
            lexer::impl_::RegexParser::leaf_node_t leaf = lexer::impl_::RegexParser::tree_t::castToLeafNode(n);

            auto s = leaf->get_value<std::set<char>>();

            os << "[" << std::string{s.begin(), s.end()} << "]" << "\n";
        } else {
            lexer::impl_::RegexParser::internal_node_t as_internal = lexer::impl_::RegexParser::tree_t::castToInternalNode(n);
            os << " {\n";
            stk.push({});
            typeof(stk) reverse;
            for (const auto &child: *as_internal) reverse.push(child);
            while (!reverse.empty()) {
                stk.push(reverse.top());
                reverse.pop();
            }
            indent_level++;
        }
    }
    return os;
}
